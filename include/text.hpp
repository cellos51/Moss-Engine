#pragma once

#include <SDL2/SDL.h>

#include <string>

#include "math.hpp"

class Text
{
public:
	Text();
	void setText(std::string data);
	void setTextC(char *data);
	std::string getText();
	unsigned int font = 0;
	Vector2 transform;
	SDL_Color color = {255, 255, 255};
	//Vector2 size;
private:
	std::string message;
};