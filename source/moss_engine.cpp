#include "moss_engine.hpp"

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
    SDL_Event event;
    bool running = true;

    while (running) 
    {
        while (SDL_PollEvent(&event) != 0) 
        {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_WINDOWEVENT) 
            {
                switch (event.window.event) 
                {
                    case SDL_WINDOWEVENT_MINIMIZED:
                        stopRendering = true;
                        break;
                    case SDL_WINDOWEVENT_RESTORED:
                        stopRendering = false;
                        break;
                }
            }

            //ImGui_ImplSDL2_ProcessEvent(&e);
        }

        if (stopRendering) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
        float time = SDL_GetTicks() / 1000.0f;

        Entity entity1;
        Entity entity2;

        entity1.transform.position = glm::vec3(0.0f, 0.0f, -4.0f);
        entity1.transform.rotation = glm::angleAxis(time, glm::vec3(0.0f, 1.0f, 0.0f));
        entity2.transform.position = glm::vec3(1.0f, 0.0f, -4.0f);

        renderer->drawEntity(&entity1);
        renderer->drawEntity(&entity2);

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