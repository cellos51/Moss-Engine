#pragma once

#include "physicsentity.hpp"
#include <SDL2/SDL_mixer.h>

class Player : public PhysicsEntity // will resume work tomorrow :(
{
public:
	//using PhysicsEntity::PhysicsEntity;
	Player();
	~Player();
	bool onWall() const;
	void update(double deltaTime);
	void fixedUpdate(double deltaTime) override;
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
	float walljumpTimeLeft = 0.0f;
	float walljumpTimeRight = 0.0f;
	float elapsedTime = 0.0f;
	Vector2 cachedVelocity = Vector2(0.0f, 0.0f);
	bool stepped = false;
	bool hitGround = false;
	Mix_Chunk* footStep[5] = { NULL };
	Mix_Chunk* landing[3] = { NULL };
	Mix_Chunk* jump[2] = { NULL };
	Mix_Chunk* slide = NULL;
};

class NetPlayer : public Entity // soulless husk puppeteered by forces beyond
{
public:
	//using PhysicsEntity::PhysicsEntity;
	NetPlayer();
	~NetPlayer();
	void update();
	float lerpSpeed = 0.1f;
	Vector2 targetPos = Vector2(0.0, 0.0);
	Vector2 velocity = Vector2(0.0, 0.0);
	int mirror = 0;
	bool resetAnimation = false;
	bool walljumping = false;
	bool OnGround = false;
private: // most of this shit if for sound effects and animations
	float elapsedTime = 0.0f;
	bool stepped = false;
	bool hitGround = false;
	Mix_Chunk* footStep[5] = { NULL };
	Mix_Chunk* landing[3] = { NULL };
	Mix_Chunk* jump[2] = { NULL };
	Mix_Chunk* slide = NULL;
};

struct PlayerData
{
	Vector2 position;
	Vector2 velocity;
	bool onGround;
	bool walljumping;
};