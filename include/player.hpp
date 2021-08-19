#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"
#include "entity.hpp"

class Player : public Entity
{
public:
	//using Entity::Entity;
	Player(Vector2 p_pos, SDL_Texture* p_tex, Vector2 p_size);
	Vector2 gravity = Vector2(0.0, 1.0);
	float friction = 0;
	float dragX = 0.10;
	float dragY = 0.01;
	float bounciness = 0; // 0.8
	bool phys = true;
	void init();
	void update();
};
