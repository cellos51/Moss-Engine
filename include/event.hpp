#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace Event
{
	void PollEvent();
	bool AppQuit();
	bool MousePressed(int i);
	bool MouseDown(int i);
	float MouseWheel();
	bool KeyPressed(int i);
	bool KeyDown(int i);
	bool TextInputEnabled(bool i);
	bool TextInputEnabled();
	bool TextSubmitted();
	std::string GetInputText();
	void SetInputText(std::string i);
}