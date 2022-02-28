#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"

class Entity // use this if you'd like an static non-moving entity
{
public:
	Entity(Vector2 p_pos, SDL_Texture* p_tex, Vector2 p_size);
	Entity(SDL_Texture* p_tex, Vector2 p_size);
	Entity(Vector2 p_pos, Vector2 p_size);
	Entity(Vector2 p_size);
	bool colUp = true, colDown = true, colLeft = true, colRight = true;
	void init();
	SDL_Texture* getTex();
	void setTex(SDL_Texture* p_tex);
	Vector2 transform;
	SDL_Texture* tex;
	Vector2 size;
	SDL_Rect offset;
	SDL_Rect texturePos;
};