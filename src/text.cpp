#include "text.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <iostream>

Text::Text()
{

}

void Text::setText(std::string data)
{
	message = data;
}

void Text::setTextC(char *data)
{
	std::string str(data);
	message = str;
}

std::string Text::getText()
{
	return message;
}