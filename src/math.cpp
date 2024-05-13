#include "math.hpp"
#include "math.h"

#include <SDL2/SDL.h>

double timeScale = 1;

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

float Vector2::magnitude()
{
   return sqrt(pow(x,2) + pow(y,2));
}

Color4::Color4() : r(0.0), g(0.0), b(0.0), a(0.0)
{

}

Color4::Color4(float p_r, float p_g, float p_b, float p_a) : r(p_r), g(p_g), b(p_b), a(p_a)
{

}

//float std::clamp(float value, float min, float max) // i don't need this because SDL has this function built in :P
//{
//   if (value > max)
//   {
//      value = max;
//   }
//
//   if (value < min)
//   {
//     value = min;
//   }
//
//   return value;
//}

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double delta = 0.0001;
//double minfps = 60.0;

void Time::Tick()
{
   LAST = NOW;
   NOW = SDL_GetPerformanceCounter();

   delta = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() ) * timeScale;

   //if(delta > 1000 / minfps)
   //{
   //   delta = 1000 / minfps; // slow down if the computer is too slow
   //}
}

double Time::deltaTime()
{
	return delta;
}

void Time::setTimeScale(double scale)
{
   timeScale = scale;
   return;
}

//float std::lerp(float a, float b, float f)
//{
//    return a + f * (b - a);
//}

float std::rad2deg(float value)
{
   return value * (M_PI / 180);
}