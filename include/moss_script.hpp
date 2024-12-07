#pragma once

#include <SDL3/SDL.h>

namespace script
{
    bool init(SDL_Window* window);
    void processUpdate(float deltaTime);
    void processFixedUpdate(float deltaTime);
    void cleanup();
}