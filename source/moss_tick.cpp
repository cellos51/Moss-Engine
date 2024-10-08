#include "moss_tick.hpp"

#include <SDL2/SDL_timer.h>

static double deltaTimeValue = 0.0;
static double elapsedTime = 0.0;

void tick::tick()
{
    static Uint64 last = SDL_GetPerformanceCounter();
    static double frequency = SDL_GetPerformanceFrequency();

    Uint64 now = SDL_GetPerformanceCounter();
    deltaTimeValue = static_cast<double>(now - last) / frequency;
    last = now;

    elapsedTime += deltaTimeValue;
}

float tick::deltaTime()
{
    return static_cast<float>(deltaTimeValue);
}

double tick::deltaTime64()
{
    return deltaTimeValue;
}

float tick::time()
{
    return static_cast<float>(elapsedTime);
}

double tick::time64()
{
    return elapsedTime;
}