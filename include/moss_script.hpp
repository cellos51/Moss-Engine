#pragma once

#include <SDL3/SDL.h>
#include <moss_renderer.hpp>

namespace script
{
    bool init(SDL_Window* window, Renderer* renderer);
    void processUpdate(float deltaTime);
    void processFixedUpdate(float deltaTime);
    void cleanup();
}