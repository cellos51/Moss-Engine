#pragma once

#include <SDL2/SDL.h>

enum MouseButton {SDLK_LEFTMOUSE = 0, SDLK_RIGHTMOUSE, SDLK_MIDDLEMOUSE};

enum KeyButton {SDLK_LEFTARROW = 0, SDLK_RIGHTARROW, SDLK_UPARROW, SDLK_DOWNARROW};

namespace Event
{
	void PollEvent();
	bool AppQuit();
	bool KeyPressed(KeyButton i);
	bool MousePressed(MouseButton i);
}