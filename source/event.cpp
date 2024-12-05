#include "event.hpp"

#include <SDL3/SDL_events.h>

#include <unordered_map>

static bool quit = false;
static bool minimized = false;

static bool keyPressedStates[SDL_SCANCODE_COUNT] = {0};
static bool keyHeldStates[SDL_SCANCODE_COUNT] = {0};
static bool keyReleasedStates[SDL_SCANCODE_COUNT] = {0};

void event::pollEvent()
{
    memset(keyPressedStates, 0, sizeof(keyPressedStates));
    memset(keyReleasedStates, 0, sizeof(keyReleasedStates));

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
            case SDL_EVENT_WINDOW_MINIMIZED:
                minimized = true;
                break;
            case SDL_EVENT_WINDOW_RESTORED:
                minimized = false;
                break;
            case SDL_EVENT_QUIT:
                quit = true;
                break;
        }
    }
}

bool event::shouldQuit()
{
    return quit;
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