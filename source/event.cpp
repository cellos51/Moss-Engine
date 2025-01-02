#include "event.hpp"

#include <SDL3/SDL_events.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <iostream>

static bool isQuitting = false;
static bool minimized = false;

static bool keyPressedStates[SDL_SCANCODE_COUNT] = {0};
static bool keyHeldStates[SDL_SCANCODE_COUNT] = {0};
static bool keyReleasedStates[SDL_SCANCODE_COUNT] = {0};

static bool mouseButtonPressedStates[6] = {0}; // tbh this is fucking stupid but sdl indexes mouse buttons from 1 to 5 so we need to allocate 6. this is [bad word] because we only need 5.
static bool mouseButtonHeldStates[6] = {0};
static bool mouseButtonReleasedStates[6] = {0};

static glm::vec2 mousePosition = {0, 0};
static glm::vec2 mouseDelta = {0, 0};

void event::pollEvent()
{
    memset(keyPressedStates, 0, sizeof(keyPressedStates));
    memset(keyReleasedStates, 0, sizeof(keyReleasedStates));

    memset(mouseButtonPressedStates, 0, sizeof(mouseButtonPressedStates));
    memset(mouseButtonReleasedStates, 0, sizeof(mouseButtonReleasedStates));
    mouseDelta = {0, 0};

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_KEY_DOWN:
                if (event.key.repeat) break;
                keyPressedStates[event.key.scancode] = true;
                keyHeldStates[event.key.scancode] = true;
                break;
            case SDL_EVENT_KEY_UP:
                keyReleasedStates[event.key.scancode] = true;
                keyHeldStates[event.key.scancode] = false;
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                mouseButtonPressedStates[event.button.button] = true;
                mouseButtonHeldStates[event.button.button] = true;
                break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
                mouseButtonReleasedStates[event.button.button] = true;
                mouseButtonHeldStates[event.button.button] = false;
                break;
            case SDL_EVENT_MOUSE_MOTION:
                mousePosition = {event.motion.x, event.motion.y};
                mouseDelta = {event.motion.xrel, event.motion.yrel};
                break;
            case SDL_EVENT_WINDOW_MINIMIZED:
                minimized = true;
                break;
            case SDL_EVENT_WINDOW_RESTORED:
                minimized = false;
                break;
            case SDL_EVENT_QUIT:
                isQuitting = true;
                break;
        }
    }
}

void event::quit()
{
    isQuitting = true;
}

bool event::shouldQuit()
{
    return isQuitting;
}

bool event::isWindowMinimized()
{
    return minimized;
}

bool event::isKeyPressed(SDL_Scancode key)
{
    return keyPressedStates[key];
}

bool event::isKeyHeld(SDL_Scancode key)
{
    return keyHeldStates[key];
}

bool event::isKeyReleased(SDL_Scancode key)
{
    return keyReleasedStates[key];
}

bool event::isMouseButtonPressed(int button)
{
    return mouseButtonPressedStates[button];
}

bool event::isMouseButtonHeld(int button)
{
    return mouseButtonHeldStates[button];
}

bool event::isMouseButtonReleased(int button)
{
    return mouseButtonReleasedStates[button];
}

glm::vec2 event::getMousePosition()
{
    return mousePosition;
}

glm::vec2 event::getMouseDelta()
{
    return mouseDelta;
}