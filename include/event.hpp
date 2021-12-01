#pragma once

#include <SDL2/SDL.h>

namespace Event
{
	void PollEvent();
	bool AppQuit();
	bool MousePressed(int i);
	bool MouseDown(int i);
	bool KeyPressed(int i);
	bool KeyDown(int i);
}