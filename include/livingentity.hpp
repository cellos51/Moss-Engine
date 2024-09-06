#pragma once

#include "physicsentity.hpp"

class LivingEntity : public PhysicsEntity
{
	using PhysicsEntity::PhysicsEntity;
	virtual void update(double deltaTime) {}
	float health = 100.0f;
	float regen = 0.0f;
};