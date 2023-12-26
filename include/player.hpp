#pragma once

#include "physicsentity.hpp"
#include <SDL2/SDL_mixer.h>

class Player : public PhysicsEntity // will resume work tomorrow :( (i'm gonna read a manga now about like a fucking trans person or something i guess)
{
public:
	//using PhysicsEntity::PhysicsEntity;
	Player();
	~Player();
	void update() override;
	int mirror = 0;
	float groundSpeed = 0.01f;
	float airSpeed = 0.01f;
	float jumpForce = 7.0f;
	float coyoteTime = 100.0f; // time in ms
	float jumpBuffer = 180.0f; // time in ms
	bool resetAnimation = false;
private: // most of this shit if for sound effects and animations
	float airTime = 0.0f;
	float jumpTime = 0.0f;
	float elapsedTime = 0.0f;
	float cachedVelocity = 0.0f;
	bool stepped = false;
	bool hitGround = false;
	Mix_Chunk* footStep[5] = { NULL };
	Mix_Chunk* landing[3] = { NULL };
	Mix_Chunk* jump[2] = { NULL };
};

