#include "moss_script.hpp"

#include "event.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <filesystem>

sol::state lua;

sol::function update;
sol::function fixedUpdate;

bool script::init()
{
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string, sol::lib::table);

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

    update = lua["Update"];
    if (!update.valid()) 
    {
        std::cerr << "Failed to find 'Update' function in Lua script." << std::endl;
        return false;
    }

    fixedUpdate = lua["FixedUpdate"];
    if (!fixedUpdate.valid()) 
    {
        std::cerr << "Failed to find 'FixedUpdate' function in Lua script." << std::endl;
        return false;
    }

    return true;
}

void script::processUpdate(float deltaTime)
{
    update(deltaTime);
}

void script::processFixedUpdate(float deltaTime)
{
    fixedUpdate(deltaTime);
}

void script::cleanup()
{
    lua.collect_garbage();
}