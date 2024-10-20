#include "moss_engine.hpp"

#include "event.hpp"
#include "tick.hpp"
#include "moss_entity.hpp"
#include "moss_script.hpp"

#include <iostream>
#include <chrono>
#include <thread>

bool MossEngine::init(int argc, char* argv[])
{
    // Check for command line arguments
    bool force_vulkan = true; // Default to Vulkan
    bool force_opengl = false;

    for (int i = 0; i < argc; i++) 
    {
        if (strcmp(argv[i], "-vulkan") == 0) 
        {
            force_vulkan = true;
            force_opengl = false;
        } 
        else if (strcmp(argv[i], "-opengl") == 0) 
        {
            force_opengl = true;
            force_vulkan = false;
        }
    }

    // Initialize
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_WindowFlags window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_RESIZABLE);

    if (force_vulkan) 
    {
        window_flags = static_cast<SDL_WindowFlags>(window_flags | SDL_WINDOW_VULKAN);
    }
    else if (force_opengl) 
    {
        window_flags = static_cast<SDL_WindowFlags>(window_flags | SDL_WINDOW_OPENGL);
    }

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
        std::string new_title = std::string(title) + " (Vulkan)";
        SDL_SetWindowTitle(window, new_title.c_str());

        renderer = std::make_unique<VulkanRenderer>();
    }
    else if (window_flags & SDL_WINDOW_OPENGL)
    {
        const char* title = SDL_GetWindowTitle(window);
        std::string new_title = std::string(title) + " (OpenGL)";
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

    if (!script::init()) 
    {
        std::cerr << "Failed to initialize lua.\n";
        return false;
    }

    isInitialized = true;
    return true;
}

void MossEngine::run()
{
    if (!isInitialized) { return; }

    while (!event::shouldQuit())
    {
        event::pollEvent();
        tick::tick();

        if (event::isWindowMinimized()) 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        update(tick::deltaTime());

        double fixedDeltaTime = 1.0 / tickrate;
        static double accumulator = 0.0;
        accumulator += tick::deltaTime64();

        while (accumulator >= fixedDeltaTime)
        {
            fixed_update(static_cast<float>(fixedDeltaTime));
            accumulator -= fixedDeltaTime;
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
            std::cout << 1.0f / tick::deltaTime() << " fps\n";
        }

        renderer->drawEntity(&entity1);

        if(!renderer->drawFrame()) {return;}
    }
}

void MossEngine::update(float deltaTime) 
{
    script::processUpdate(deltaTime);
}

void MossEngine::fixed_update(float deltaTime)
{
    script::processFixedUpdate(deltaTime);
}

void MossEngine::cleanup()
{
    if (isInitialized) 
    {
        script::cleanup(); // Not super necessary, but good practice and if i ever add more cleanup code to script::cleanup() it will be useful
        renderer->cleanup();
        SDL_DestroyWindow(window);
    }

    renderer.reset();
    SDL_Quit();
}