#pragma once 

#include <SDL3/SDL_scancode.h>

namespace event
{
    void pollEvent();
    bool shouldQuit();
    bool isWindowMinimized();
    bool isKeyPressed(SDL_Scancode key); // Returns true for the first frame the key is pressed
    bool isKeyHeld(SDL_Scancode key); // Returns true for every frame the key is held
    bool isKeyReleased(SDL_Scancode key); // Returns true for the first frame the key is released
}