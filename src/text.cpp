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
	messageSurface = TTF_RenderText_Blended(swansea, data.c_str(), color);
	TTF_SizeText(swansea, data.c_str(), &w, &h);
	size.x = w;
	size.y = h;
}

void Text::setTextC(char *data)
{
	std::string str(data);
	message = str;
	int w, h;
	SDL_FreeSurface(messageSurface);
	messageSurface = TTF_RenderText_Blended(swansea, data, color);
	TTF_SizeText(swansea, data, &w, &h);
	size.x = w;
	size.y = h;
}

std::string Text::getText()
{
	return message;
}