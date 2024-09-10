// This is the header file for the Window class, which is used to create a window and initialize a renderer. 
// The Window class is responsible for creating an SDL window and initializing a renderer based on the flags passed to the constructor. 
// It also handles cleaning up the renderer and the SDL window when the object is destroyed.

#pragma once

#include "renderer.hpp"

#include <SDL2/SDL.h>

#include <string>

class Window
{
public:
    Window(int width, int height, std::string title, Uint32 flags);
    void render();
    ~Window();
private:
    SDL_Window* window;
    IRenderer* renderer;
};
