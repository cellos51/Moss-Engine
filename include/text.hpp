#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "math.hpp"

class Text
{
public:
	Text();
	void setText(std::string data);
	void setTextC(char *data);
	std::string getText();
	TTF_Font* font = NULL;
	Vector2 transform;
	SDL_Color color = {255, 255, 255};
	Vector2 size;
private:
	std::string message;
};