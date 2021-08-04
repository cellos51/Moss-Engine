#pragma once

#include <SDL2/SDL.h>

namespace Event
{
	void PollEvent();
	bool AppQuit();
	bool KeyPressed(int i);
	bool MousePressed(int i);
}