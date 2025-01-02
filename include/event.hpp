#pragma once 

#include <SDL3/SDL_scancode.h>
#include <glm/glm.hpp>

namespace event
{
    void pollEvent();
    void quit();
    bool shouldQuit();
    bool isWindowMinimized();
    bool isKeyPressed(SDL_Scancode key); // Returns true for the first frame the key is pressed
    bool isKeyHeld(SDL_Scancode key); // Returns true for every frame the key is held
    bool isKeyReleased(SDL_Scancode key); // Returns true for the first frame the key is released
    bool isMouseButtonPressed(int button); // Returns true for the first frame the mouse button is pressed
    bool isMouseButtonHeld(int button); // Returns true for every frame the mouse button is held
    bool isMouseButtonReleased(int button); // Returns true for the first frame the mouse button is released
    glm::vec2 getMousePosition();
    glm::vec2 getMouseDelta();
}