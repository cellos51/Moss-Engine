#include "event.hpp"

#include <SDL_events.h>

#include <unordered_map>

static bool quit = false;
static bool minimized = false;

static bool keyPressedStates[SDL_NUM_SCANCODES] = {0};
static bool keyHeldStates[SDL_NUM_SCANCODES] = {0};

void event::pollEvent()
{
    memset(keyPressedStates, 0, sizeof(keyPressedStates));

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
                if (event.key.repeat) break;
                keyPressedStates[event.key.keysym.scancode] = true;
                keyHeldStates[event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                keyHeldStates[event.key.keysym.scancode] = false;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_MINIMIZED:
                        minimized = true;
                        break;
                    case SDL_WINDOWEVENT_RESTORED:
                        minimized = false;
                        break;
                }
                break;
            case SDL_QUIT:
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