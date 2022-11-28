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
	unsigned int font = 3;
	int fontSize = 15;
	Vector2 transform = Vector2(50,50);
	SDL_Color color = {255, 255, 255};
	//Vector2 size;
	std::vector<Entity> characters;
private:
	std::string message;
	
};