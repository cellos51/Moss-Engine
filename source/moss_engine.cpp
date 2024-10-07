#include "moss_engine.hpp"

#include "moss_event.hpp"
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
        std::cerr << "UNSUPPORED RENDERER DETECTED! ISSUING DIRECT AIR STRIKE ON THE USER!\n";
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

void MossEngine::run()
{
    while (!event::shouldQuit())
    {
        event::pollEvent();

        if (event::isWindowMinimized()) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        float time = SDL_GetTicks() / 1000.0f;

        if (event::isKeyHeld(SDL_SCANCODE_SPACE)) 
        {
            time *= 3;
        }
        
        Entity entity1;
        Entity entity2;
        Entity entity3;

        entity1.transform.position = glm::vec3(sin(time) * 2, 0.0f, -4.0f);
        entity2.transform.position = glm::vec3(0.0f, 0.0f, sin(time * 1.5) + -4.0f);
        entity3.transform.position = glm::vec3(0.0f, sin(time* 2.3) * 3, -4.0f);

        renderer->drawEntity(&entity1);
        renderer->drawEntity(&entity2);
        renderer->drawEntity(&entity3);

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