#include "moss_engine.hpp"

#include "moss_event.hpp"
#include "moss_tick.hpp"
#include "moss_entity.hpp"

#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>

MossEngine* loadedEngine = nullptr;

MossEngine& MossEngine::Get() { return *loadedEngine; }
bool MossEngine::init()
{
    assert(loadedEngine == nullptr);
    loadedEngine = this;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    window = SDL_CreateWindow
    (
        "Moss Engine",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280,
        720,
        window_flags
    );

    if (window_flags & SDL_WINDOW_VULKAN) 
    {
        renderer = std::make_unique<VulkanRenderer>();
    }
    else 
    {
        std::cerr << "UNSUPPORTED RENDERER DETECTED! ISSUING DIRECT AIR STRIKE ON THE USER!\n";
        return false;
    }

    if(!renderer->init(window))
    {
        std::cerr << "Failed to initialize renderer.\n";
        return false;
    }

    isInitialized = true;
    return true;
}

Entity entity1;

void MossEngine::run()
{
    while (!event::shouldQuit())
    {
        event::pollEvent();
        tick::tick();

        if (event::isWindowMinimized()) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        // move entity with arrow keys and tick::deltaTime()
        
        if (event::isKeyHeld(SDL_SCANCODE_UP)) 
        {
            entity1.transform.position.y += 1.0f * tick::deltaTime();
        }

        if (event::isKeyHeld(SDL_SCANCODE_DOWN)) 
        {
            entity1.transform.position.y -= 1.0f * tick::deltaTime();
        }

        if (event::isKeyHeld(SDL_SCANCODE_LEFT)) 
        {
            entity1.transform.position.x -= 1.0f * tick::deltaTime();
        }

        if (event::isKeyHeld(SDL_SCANCODE_RIGHT)) 
        {
            entity1.transform.position.x += 1.0f * tick::deltaTime();
        }

        renderer->drawEntity(&entity1);

        renderer->drawFrame();
    }
}

void MossEngine::cleanup()
{
    if (isInitialized) 
    {
        renderer->cleanup();
        SDL_DestroyWindow(window);
    }

    loadedEngine = nullptr;
}