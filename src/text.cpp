#include "text.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

Text::Text()
{

}

void Text::setText(std::string data)
{
	message = data;
	int w, h;
	SDL_FreeSurface(messageSurface);
	messageSurface = TTF_RenderText_Solid(swansea, data.c_str(), color);
	TTF_SizeText(swansea, data.c_str(), &w, &h);
	size.x = w;
	size.y = h;
}

std::string Text::getText()
{
	return message;
}