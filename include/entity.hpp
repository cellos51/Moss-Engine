#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"

class Entity // i don't feel like figuring out how to make this non instantiable. so dont instatiate it pls :) Edit: i ended up not following that rule :P
{
public:
	Entity(Vector2 p_pos, SDL_Texture* p_tex, Vector2 p_size);
	Entity(SDL_Texture* p_tex, Vector2 p_size);
	Entity(Vector2 p_pos, Vector2 p_size);
	Entity(Vector2 p_size);
	Vector2 velocity;
	Vector2 gravity = Vector2(0.0, 1.0);
	float friction = 1;
	float dragX = 0.5;
	float dragY = 0.5;
	float bounciness = 0.8;
	bool phys = false;
	bool colUp = true, colDown = true, colLeft = true, colRight = true;
	void init();
	SDL_Texture* getTex();
	void setTex(SDL_Texture* p_tex);
	void getCol(Entity& p_ent);
	void physics(bool p_phys);
	void update();
	bool OnGround = false;
	Vector2 transform;
	SDL_Texture* tex;
	Vector2 size;
	SDL_Rect currentFrame;
	SDL_Rect texturePos;
};