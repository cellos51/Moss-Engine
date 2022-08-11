#include "math.hpp"

#include <SDL2/SDL.h>



Vector2::Vector2() : x(0.0), y(0.0)
{

}

Vector2::Vector2(float p_x, float p_y) : x(p_x), y(p_y)
{

}

Vector2 Vector2::lerp(Vector2 a, Vector2 b, float time)
{
   a.x = a.x + time * (b.x - a.x);

   a.y = a.y + time * (b.y - a.y);

   x = a.x;

   y = a.y;

   return a;
}

float std::clamp(float value, float min, float max)
{
   if (value > max)
   {
      value = max;
   }

   if (value < min)
   {
     value = min;
   }

   return value;
}

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
int minfps = 60;
double delta = 0.0001;

void Time::Tick()
{
   LAST = NOW;
   NOW = SDL_GetPerformanceCounter();

   delta = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );

   if(delta > 1000 / minfps)
   {
      delta = 1000 / minfps; // slow down if the computer is too slow
   }
}

double Time::deltaTime()
{
	return delta;
}

float std::lerp(float a, float b, float f)
{
    return a + f * (b - a);
}