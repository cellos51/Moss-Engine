#include <cstdint>
#include <SDL2/SDL.h>

#include "math.hpp"

Vector2::Vector2() : x(0.0), y(0.0)
{

}

Vector2::Vector2(float p_x, float p_y) : x(p_x), y(p_y)
{

}


Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double delta = 0.0001;

void Time::Tick()
{
   LAST = NOW;
   NOW = SDL_GetPerformanceCounter();

   delta = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
}

double Time::deltaTime()
{
	return delta;
}