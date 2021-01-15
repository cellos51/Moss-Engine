#pragma once

#include <SDL2/SDL.h>

enum MouseButton {SDLK_LEFTMOUSE = 0, SDLK_RIGHTMOUSE, SDLK_MIDDLEMOUSE};

namespace Event
{
	void PollEvent();
	bool AppQuit();
	bool KeyPressed();
	bool MousePressed(MouseButton i);
}