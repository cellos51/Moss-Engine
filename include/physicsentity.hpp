#pragma once
#include "entity.hpp"

#include <SDL2/SDL.h>

#include "math.hpp"

class PhysicsEntity : public Entity // use this if you'd like an entity with premade physics
{
public:
	using Entity::Entity;
	Vector2 velocity;
	Vector2 gravity = Vector2(0.0, 0.2f);
	float friction = 0; // 1
	float dragX = 0.3f; // 0.01
	float dragY = 0.01f; // 0.01
	float bounciness = 0; // 0.8
	bool phys = true;
	bool interpolate = false;
	void getCol(Entity& p_ent, double deltaTime);
	void physics(bool p_phys, double deltaTime);
	virtual void fixedUpdate(double deltaTime)
	{
		PhysicsEntity::physics(phys, deltaTime);
	}
	bool leftTouch = false;
	bool rightTouch = false;
	bool topTouch = false;
	bool OnGround = false;
};