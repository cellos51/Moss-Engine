#pragma once
#include "livingentity.hpp"

#include <SDL2/SDL.h>

#include "math.hpp"

class Enemy : public LivingEntity
{
public:
	//using Entity::Entity;
	Enemy(Vector2 p_pos, unsigned int p_tex, Vector2 p_size);
	Vector2 movementDir = Vector2(0,0);
	void update();
};
