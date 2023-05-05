#pragma once

#include <SDL2/SDL.h>

#include "math.hpp"

class Entity // use this if you'd like an static non-moving entity
{
public:
	Entity(Vector2 p_pos, unsigned int p_tex, Vector2 p_size);
	Entity(unsigned int p_tex, Vector2 p_size);
	Entity(Vector2 p_pos, Vector2 p_size);
	Entity(Vector2 p_size);
	bool colUp = true, colDown = true, colLeft = true, colRight = true;
	void init();
	unsigned int getTex();
	void setTex(unsigned int p_tex);
	bool intersecting(Entity p_ent);
	Vector2 transform;
	unsigned int tex;
	unsigned int layer;
	Vector2 size;
	float rotation = 0.0f;
	SDL_Rect offset;
	SDL_Rect texturePos;
	Color4 luminosity = Color4(0.0,0.0,0.0,1.0);
};