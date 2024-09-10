#include "window.hpp"

#include "vulkan_renderer.hpp"

#include <SDL2/SDL_vulkan.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>

Window::Window(int width, int height, std::string title, Uint32 flags)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    if ((flags & SDL_WINDOW_VULKAN) == SDL_WINDOW_VULKAN) 
    {
        title += " (Vulkan)";
        SDL_Vulkan_LoadLibrary(nullptr);
        renderer = new VulkanRenderer();
    }
    else if ((flags & SDL_WINDOW_OPENGL) == SDL_WINDOW_OPENGL) 
    {
        title += " (OpenGL)";
        SDL_GL_LoadLibrary(nullptr);
        renderer = nullptr; //TODO: Implement OpenGLRenderer
    }
    else
    {
        SDL_Quit();
        throw std::runtime_error("Window must be created with either SDL_WINDOW_VULKAN or SDL_WINDOW_OPENGL");
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (window == nullptr) 
    {
        SDL_Quit();
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
    }

    renderer->initialize(window);
}

void Window::render()
{
    renderer->render();
}

Window::~Window()
{
    delete renderer;
    SDL_DestroyWindow(window);
    SDL_Quit();
}