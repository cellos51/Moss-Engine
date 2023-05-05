#pragma once
#include "livingentity.hpp"
#include "global.hpp"

#include <SDL2/SDL.h>

#include "math.hpp"

class Player : public LivingEntity
{
public:
	//using Entity::Entity;
	Player(Vector2 p_pos, unsigned int p_tex, Vector2 p_size);
	Vector2 movementDir = Vector2(0,0);
	Vector2 hitDir = Vector2(0,1);
	float animationFrame = 0;
	float animationFrameSword = 0;
	int lastFrame = 0;
	void update();
};
