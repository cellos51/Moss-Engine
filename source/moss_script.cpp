#include "moss_script.hpp"

#include "event.hpp"
#include "moss_entity.hpp"
#include "moss_camera.hpp"
#include "moss_mesh_instance.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

#include <filesystem>
#include <vector>

static sol::state lua;

static std::vector<sol::function> update_callbacks;
static std::vector<sol::function> fixed_update_callbacks;

class CallbackConnection
{
    public:
        CallbackConnection(sol::function callback, std::vector<sol::function>* callbackList) : callback(callback), callbackList(callbackList) 
        {
            callbackList->push_back(callback);
        }
        void Disconnect() 
        {
            callbackList->erase(std::remove(callbackList->begin(), callbackList->end(), callback), callbackList->end());
        }
        bool IsConnected() { return connected; }
    private:
        sol::function callback;
        std::vector<sol::function>* callbackList;
        bool connected = true;
};

class CallbackSignal
{
    public:
        CallbackSignal(std::vector<sol::function>* callbackList) : callbackList(callbackList) {}
        CallbackConnection Connect(sol::function callback) { return CallbackConnection(callback, callbackList); }
    private:
        std::vector<sol::function>* callbackList;
};

class CursorProperties // This needs to be a class for some reason. SOL doesn't work if we bind the functions directly to lua.
{
    public:
        CursorProperties(SDL_Window* window) : window(window) {}
        bool GetLocked() const { return SDL_GetWindowRelativeMouseMode(window); }
        void SetLocked(bool value) { SDL_SetWindowRelativeMouseMode(window, value); }
        bool GetVisible() const { return SDL_CursorVisible(); }
        void SetVisible(bool value) { value ? SDL_ShowCursor() : SDL_HideCursor(); }
    private:
        SDL_Window* window = nullptr;
};

bool script::init(SDL_Window* window, Renderer* renderer)
{
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string, sol::lib::table);

    // Moss Engine bindings
    lua.new_usertype<Entity>("Entity",
        "new", sol::factories([]() { return new Entity(); }),
        "Destroy", &Entity::destroy,
        "Transform", &Entity::transform
    );

    lua.new_usertype<Transform>("Transform",
        "LocalEulerAngles", sol::property(&Transform::getLocalEulerAngles, &Transform::setLocalEulerAngles),
        "LocalPosition", sol::property(&Transform::getLocalPosition, &Transform::setLocalPosition),
        "LocalRotation", sol::property(&Transform::getLocalRotation, &Transform::setLocalRotation),
        "LocalScale", sol::property(&Transform::getLocalScale, &Transform::setLocalScale),

        "EulerAngles", sol::property(&Transform::getEulerAngles, &Transform::setEulerAngles),
        "Position", sol::property(&Transform::getPosition, &Transform::setPosition),
        "Rotation", sol::property(&Transform::getRotation, &Transform::setRotation),

        "Forward", sol::property(&Transform::getForward),
        "Right", sol::property(&Transform::getRight),
        "Up", sol::property(&Transform::getUp),
        
        "Parent", sol::property(&Transform::getParent, &Transform::setParent)
    );

    lua.new_usertype<Camera>("Camera",
        "new", sol::factories([]() { return new Camera(); }),
        "Destroy", &Camera::destroy,
        "Transform", &Camera::transform,
        "ViewMatrix", &Camera::getViewMatrix,
        "FieldOfView", &Camera::fov,
        "NearClipPlane", &Camera::near_clip,
        "FarClipPlane", &Camera::far_clip
    );

    lua.new_usertype<MeshInstance>("MeshInstance",
        "new", sol::factories([]() { return new MeshInstance(); }),
        "Destroy", &MeshInstance::destroy,
        "Transform", &MeshInstance::transform,
        "Mesh", &MeshInstance::mesh
    );

    lua.new_usertype<CallbackConnection>("CallbackConnection",
        "Connected", sol::property(&CallbackConnection::IsConnected),
        "Disconnect", &CallbackConnection::Disconnect
    );

    lua.new_usertype<CallbackSignal>("CallbackSignal",
        "Connect", &CallbackSignal::Connect
    );

    sol::table callback_table = lua.create_named_table("Callback");
    callback_table.set("Update", CallbackSignal(&update_callbacks));
    callback_table.set("FixedUpdate", CallbackSignal(&fixed_update_callbacks));

    sol::table renderer_table = lua.create_named_table("Renderer");
    renderer_table.set("Camera", renderer->camera);

    lua.set("Cursor", CursorProperties(window));
    lua.new_usertype<CursorProperties>("CursorProperties",
        "Locked", sol::property(&CursorProperties::GetLocked, &CursorProperties::SetLocked),
        "Visible", sol::property(&CursorProperties::GetVisible, &CursorProperties::SetVisible)
    );

    sol::table input_table = lua.create_named_table("Input");
    input_table.set_function("IsKeyPressed", event::isKeyPressed);
    input_table.set_function("IsKeyHeld", event::isKeyHeld);
    input_table.set_function("IsKeyReleased", event::isKeyReleased);
    input_table.set_function("IsMouseButtonPressed", event::isMouseButtonPressed);
    input_table.set_function("IsMouseButtonHeld", event::isMouseButtonHeld);
    input_table.set_function("IsMouseButtonReleased", event::isMouseButtonReleased);
    input_table.set_function("GetMousePosition", event::getMousePosition);
    input_table.set_function("GetMouseDelta", event::getMouseDelta);

    sol::table scan_code_table = lua.create_named_table("ScanCode");
    sol::table scan_code_metatable = lua.create_table_with();

    for (int i = 0; i < SDL_SCANCODE_COUNT; i++)
    {
        const char* name = SDL_GetScancodeName(static_cast<SDL_Scancode>(i));
        if (name) 
        {
            scan_code_metatable[name] = static_cast<SDL_Scancode>(i);
        }
    }

    scan_code_metatable[sol::meta_function::new_index] = NULL;
    scan_code_metatable[sol::meta_function::index] = scan_code_metatable;

    scan_code_table[sol::metatable_key] = scan_code_metatable;
    
    // GLM bindings
    lua.new_usertype<glm::vec2>("Vector2",
        "new", sol::factories([](float x, float y) { return glm::vec2(x, y); }),

        "zero", sol::factories([]() { return glm::vec2(0.0f, 0.0f); }),
        "one", sol::factories([]() { return glm::vec2(1.0f, 1.0f); }),
        "xAxis", sol::factories([]() { return glm::vec2(1.0f, 0.0f); }),
        "yAxis", sol::factories([]() { return glm::vec2(0.0f, 1.0f); }),
        "X", &glm::vec2::x,
        "Y", &glm::vec2::y,
        "Magnitude", sol::property([](const glm::vec2& vec) { return glm::length(vec); }),
        "Unit", sol::property([](const glm::vec2& vec) { return glm::length(vec) > 0.0f ? glm::normalize(vec) : glm::vec2(0.0f); }),

        sol::meta_function::addition, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator+),
        sol::meta_function::subtraction, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator-),
        sol::meta_function::multiplication, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator*),
        sol::meta_function::multiplication, sol::resolve<glm::vec2(const glm::vec2&, float)>(glm::operator*),
        sol::meta_function::division, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator/),
        sol::meta_function::division, sol::resolve<glm::vec2(const glm::vec2&, float)>(glm::operator/)
    );

    lua.new_usertype<glm::vec3>("Vector3",
        "new", sol::factories([](float x, float y, float z) { return glm::vec3(x, y, z); }),

        "zero", sol::factories([]() { return glm::vec3(0.0f); }),
        "one", sol::factories([]() { return glm::vec3(1.0f); }),
        "xAxis", sol::factories([]() { return glm::vec3(1.0f, 0.0f, 0.0f); }),
        "yAxis", sol::factories([]() { return glm::vec3(0.0f, 1.0f, 0.0f); }),
        "zAxis", sol::factories([]() { return glm::vec3(0.0f, 0.0f, 1.0f); }),
        "X", &glm::vec3::x,
        "Y", &glm::vec3::y,
        "Z", &glm::vec3::z,
        "Magnitude", sol::property([](const glm::vec3& vec) { return glm::length(vec); }),
        "Unit", sol::property([](const glm::vec3& vec) { return glm::length(vec) > 0.0f ? glm::normalize(vec) : glm::vec3(0.0f); }),

        sol::meta_function::addition, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator+),
        sol::meta_function::subtraction, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator-),
        sol::meta_function::multiplication, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator*),
        sol::meta_function::multiplication, sol::resolve<glm::vec3(const glm::vec3&, float)>(glm::operator*),
        sol::meta_function::division, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator/),
        sol::meta_function::division, sol::resolve<glm::vec3(const glm::vec3&, float)>(glm::operator/)
    );

    lua.new_usertype<glm::quat>("Quaternion",
        "new", sol::factories([](float x, float y, float z, float w) { return glm::quat(w, x, y, z); }),
        
        "X", &glm::quat::x,
        "Y", &glm::quat::y,
        "Z", &glm::quat::z,
        "W", &glm::quat::w
    );

    // Load Lua scripts
    for (const auto& entry : std::filesystem::recursive_directory_iterator("assets/scripts"))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".lua")
        {
            try 
            {
                lua.script_file(entry.path().string());
            } 
            catch (const sol::error& e) 
            {
                std::cerr << "Failed to load Lua script: " << entry.path() << " - " << e.what() << std::endl;
                return false;
            }
        }
    }

    return true;
}

void script::processUpdate(float deltaTime)
{
    for (const auto& callback : update_callbacks)
    {
        callback(deltaTime);
    }
}

void script::processFixedUpdate(float deltaTime)
{
    for (const auto& callback : fixed_update_callbacks)
    {
        callback(deltaTime);
    }
}

void script::cleanup()
{
    lua.collect_garbage();
}