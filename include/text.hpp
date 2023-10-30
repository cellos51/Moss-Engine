#pragma once

#include <SDL2/SDL.h>

#include <string>

#include "math.hpp"
#include "entity.hpp"
#include <vector>

class Text
{
public:
	Text();
	void setText(std::string data);
	void setTextC(char *data);
	std::string getText();
	unsigned int getLines();
	unsigned int font = -1;
	unsigned int lineLength = 10; // do -1 for like virtually infinite (int underflow y'know?) 
	int fontSize = 16;
	Vector2 transform = Vector2(0,0);
	//Vector2 size;
	Color4 luminosity = Color4(0.1, 0.1, 0.1, 0);
	std::vector<Entity> characters;
private:
	unsigned int lines = 0;
	std::string message;
	unsigned int oldLength = lineLength;
};