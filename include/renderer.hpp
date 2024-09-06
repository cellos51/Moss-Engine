// This file defines the Renderer interface, which is used to render the game to the screen.

#pragma once

#include <SDL2/SDL.h>

class IRenderer
{
public:
    virtual void initialize(SDL_Window* window) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;
    virtual ~IRenderer() = default;
};