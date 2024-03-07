#pragma once

#include "math.hpp"
#include <SDL2/SDL.h>
#include <string>

namespace Event
{
	void PollEvent();
	void AppStart();
	bool AppQuit();
	bool MousePressed(int i);
	bool MouseDown(int i);
	float MouseWheel();
	bool KeyPressed(int i);
	bool KeyDown(int i);
	bool ButtonPressed(int i);
	bool ButtonDown(int i);
	float JoyAxis(int i);
	bool TextInputEnabled(bool i);
	bool TextInputEnabled();
	bool TextSubmitted();
	std::string GetInputText();
	void SetInputText(std::string i);
}