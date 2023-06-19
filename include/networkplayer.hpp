#pragma once
#include "livingentity.hpp"
#include "global.hpp"

#include <SDL2/SDL.h>

#include "math.hpp"

class NetworkPlayer : public LivingEntity
{
public:
	//using Entity::Entity;
	NetworkPlayer(Vector2 p_pos, unsigned int p_tex, Vector2 p_size);
	float animationFrame = 0;
	int lastFrame = 0;

	unsigned int movement = 0;
	unsigned int direction = 1;

	void update();
};