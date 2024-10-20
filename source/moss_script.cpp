#include "moss_script.hpp"

#include "moss_event.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

sol::state lua;

sol::function update;
sol::function fixedUpdate;

bool script::init()
{
    try
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

        lua.script_file("assets/scripts/example.lua");

        update = lua["Update"];
        fixedUpdate = lua["FixedUpdate"];
    }
    catch (const sol::error& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

void script::processUpdate(float deltaTime)
{
    update();
}

void script::processFixedUpdate(float deltaTime)
{
    fixedUpdate();
}

void script::cleanup()
{
    lua.collect_garbage();
}