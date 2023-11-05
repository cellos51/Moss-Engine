#pragma once
#include "entity.hpp"

#include <SDL2/SDL.h>

#include "math.hpp"

class PhysicsEntity : public Entity // use this if you'd like an entity with premade physics
{
public:
	using Entity::Entity;
	Vector2 velocity;
	Vector2 gravity = Vector2(0.0, 0.3f);
	float friction = 0; // 1
	float dragX = 0.3f; // 0.01
	float dragY = 0.01f; // 0.01
	float bounciness = 0; // 0.8
	bool phys = true;
	void getCol(Entity& p_ent);
	void physics(bool p_phys);
	virtual void update()
	{
		PhysicsEntity::physics(phys);
	}
	bool OnGround = false;
};