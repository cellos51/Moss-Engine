#include "moss_script.hpp"

#include "event.hpp"
#include "moss_entity.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <filesystem>
#include <vector>

sol::state lua;

std::vector<sol::function> update_callbacks;
std::vector<sol::function> fixed_update_callbacks;

class Callback // TODO: make this less stupid 🥺
{
    public:
        Callback(sol::function callback, std::vector<sol::function>* callbackList) : callback(callback), callbackList(callbackList) {}
        void Disconnect() 
        {
            callbackList->erase(std::remove(callbackList->begin(), callbackList->end(), callback), callbackList->end());
        }
    private:
        sol::function callback;
        std::vector<sol::function>* callbackList;
};

bool script::init()
{
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string, sol::lib::table);

    // Moss Engine bindings
    lua.new_usertype<Entity>("Entity",
        "new", sol::factories([]() { return entity::createEntity();}),
        "Destroy", &entity::destroyEntity,
        "Transform", &Entity::transform
    );

    lua.new_usertype<Transform>("Transform",
        "EulerAngles", sol::property(
        [](const Transform& transform) { return glm::eulerAngles(transform.rotation); },
        [](Transform& transform, const glm::vec3& eulerAngles) { transform.rotation = glm::quat(eulerAngles); }
        ),
        "Position", &Transform::position,
        "Rotation", &Transform::rotation,
        "Scale", &Transform::scale
    );

    lua.new_usertype<Callback>("Callback",
        "Disconnect", &Callback::Disconnect
    );

    sol::table callback = lua.create_named_table("Callback");
    callback["Update"].set_function([](sol::function callback) 
    { 
        update_callbacks.push_back(callback); 
        return Callback(callback, &update_callbacks);
    });
    callback["FixedUpdate"].set_function([](sol::function callback) 
    { 
        fixed_update_callbacks.push_back(callback); 
        return Callback(callback, &fixed_update_callbacks);
    });

    sol::table input = lua.create_named_table("Input");
    input.set_function("IsKeyPressed", event::isKeyPressed);
    input.set_function("IsKeyHeld", event::isKeyHeld);

    sol::table scan_code_table = lua.create_named_table("ScanCode");
    sol::table scan_code_metatable = lua.create_table_with();

    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
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
        "Magnitude", &glm::vec3::length,
        "Unit", sol::property([](const glm::vec3& vec) { return glm::normalize(vec); }),

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