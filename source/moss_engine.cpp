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

    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

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
        const char* title = SDL_GetWindowTitle(window);
        std::string new_title = std::string(title) + " - Vulkan";
        SDL_SetWindowTitle(window, new_title.c_str());

        renderer = std::make_unique<VulkanRenderer>();
    }
    else if (window_flags & SDL_WINDOW_OPENGL)
    {
        const char* title = SDL_GetWindowTitle(window);
        std::string new_title = std::string(title) + " - OpenGL";
        SDL_SetWindowTitle(window, new_title.c_str());   

        renderer = std::make_unique<OpenGLRenderer>();
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
        static Entity entity1;

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

        if (event::isKeyPressed(SDL_SCANCODE_SPACE))
        {
            std::cout << tick::deltaTime() * 1000.0f << " ms\n";
            std::cout << 1.0f / tick::deltaTime() << " FPS\n";
        }

        renderer->drawEntity(&entity1);

        if(!renderer->drawFrame()) {return;}
    }
}

void MossEngine::cleanup()
{
    if (isInitialized) 
    {
        renderer->cleanup();
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    loadedEngine = nullptr;
}