#pragma once

#include "physicsentity.hpp"

class Player : public PhysicsEntity // will resume work tomorrow :( (i'm gonna read a manga now about like a fucking trans person or something i guess)
{
public:
	using PhysicsEntity::PhysicsEntity;
	void update() override;

	int mirror = 0;
	float speed = 0.03f;
	float jump = 7.0f;
	bool resetAnimation = false;
private:
	float elapsedTime = 0.0f;
};

