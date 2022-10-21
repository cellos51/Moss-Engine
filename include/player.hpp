#pragma once
#include "physicsentity.hpp"

#include <SDL2/SDL.h>

#include "math.hpp"

class Player : public PhysicsEntity
{
public:
	//using Entity::Entity;
	Player(Vector2 p_pos, unsigned int p_tex, Vector2 p_size);
	Vector2 gravity = Vector2(0.0, 0.0);
	float friction = 0;
	float dragX = 0.3;
	float dragY = 0.3;
	float bounciness = 0; // 0.8
	float movespeed = 1;
	bool phys = true;
	Vector2 movementDir = Vector2(0,0);
	void init();
	void update();
};
