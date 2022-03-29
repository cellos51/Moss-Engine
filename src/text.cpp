#include "text.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

Text::Text()
{

}

void Text::setText(std::string data)
{
	message = data;

	if (font != NULL)
	{
		int w, h;
		TTF_SizeText(font, data.c_str(), &w, &h);
		size.x = w;
		size.y = h;
	}
}

void Text::setTextC(char *data)
{
	std::string str(data);
	message = str;
	if (font != NULL)
	{
		int w, h;
		TTF_SizeText(font, data, &w, &h);
		size.x = w;
		size.y = h;
	}
}

std::string Text::getText()
{
	return message;
}