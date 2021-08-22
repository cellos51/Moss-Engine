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
	TTF_Font* swansea = TTF_OpenFont("assets/fonts/swansea.ttf", 24);
	SDL_Surface* messageSurface;
	SDL_Texture* messageTex;
	Vector2 transform;
	Vector2 size;
	SDL_Color color = {255, 255, 255};
private:
	std::string message;
};