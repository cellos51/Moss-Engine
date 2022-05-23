#pragma once
#include "entity.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"

class PhysicsEntity : public Entity // use this if you'd like an entity with premade physics
{
public:
	using Entity::Entity;
	Vector2 velocity;
	Vector2 gravity = Vector2(0.0, 1.0);
	float friction = 1;
	float dragX = 0.01;
	float dragY = 0.01;
	float bounciness = 0.8;
	bool phys = true;
	void getCol(Entity& p_ent);
	void physics(bool p_phys);
	void update();
	bool OnGround = false;
};