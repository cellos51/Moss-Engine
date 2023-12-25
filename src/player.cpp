#include "player.hpp"

#include "event.hpp"
#include "Windows.h"

#include <cmath>

Player::Player()
{
	size = Vector2(24, 48);

	luminosity = Color4(0.2f, 0.2f, 0.2f, 0.0f);
	layer = 2;
	texturePos.w = 64;
	texturePos.h = 64;
	offset.w = 64;
	offset.h = 64;

	offset.x -= (offset.w - size.x) / 2;
	offset.y -= offset.h - size.y;

	footStep[0] = Mix_LoadWAV("assets/audio/walk1.wav");
	footStep[1] = Mix_LoadWAV("assets/audio/walk2.wav");
	footStep[2] = Mix_LoadWAV("assets/audio/walk3.wav");
	footStep[3] = Mix_LoadWAV("assets/audio/walk4.wav");
	landing = Mix_LoadWAV("assets/audio/land.wav");
	jump = Mix_LoadWAV("assets/audio/jump.wav");
}

Player::~Player()
{
	Mix_FreeChunk(footStep[0]);
	Mix_FreeChunk(footStep[1]);
	Mix_FreeChunk(footStep[2]);
	Mix_FreeChunk(footStep[3]);
	Mix_FreeChunk(landing);
	Mix_FreeChunk(jump);
}

void Player::update()
{
	// movement

	float moveDir = 0.0f;

	float speed = 0;

	if (OnGround == true)
	{
		airTime = coyoteTime;

		speed = groundSpeed;
		dragX = 0.04f;

		if (!Event::KeyPressed(SDLK_a) && !(Event::KeyPressed(SDLK_d)))
		{
			dragX = 0.3f;
		}
	}
	else
	{
		airTime -= Time::deltaTime();

		speed = airSpeed;
		dragX = 0.03f;
	}


	if (Event::KeyPressed(SDLK_d))
	{
		moveDir += speed;

		if (velocity.x < 0)
		{
			dragX = 0.3f;
		}
	}

	if (Event::KeyPressed(SDLK_a))
	{
		moveDir -= speed;

		if (velocity.x > 0)
		{
			dragX = 0.3f;
		}
	}

	if (Event::KeyDown(SDLK_SPACE) && airTime > 0)
	{
		airTime = 0;
		velocity.y -= jumpForce;
		Mix_PlayChannel(5, jump, 0);
	}
	else if (!Event::KeyPressed(SDLK_SPACE) && !OnGround && velocity.y < 0)
	{
		velocity.y = std::lerp(velocity.y, 0.0f, Time::deltaTime() * 0.05);
	}

	velocity.x += moveDir * Time::deltaTime();

	// animation
	if (moveDir > 0)
	{
		texturePos.w = 64;
		mirror = 0;
	}
	else if (moveDir < 0)
	{
		texturePos.w = -64;
		mirror = 1;
	}

	if (OnGround == true)
	{
		if (hitGround == false)
		{
			Mix_Volume(5, cachedVelocity * 2);
			Mix_PlayChannel(5, landing, 0);
			hitGround = true;
		}

		if (std::abs(velocity.x) < 0.1)
		{
			elapsedTime += Time::deltaTime() / 200.0f;
			texturePos.x = (mirror + int(elapsedTime) % 8) * 64;
			texturePos.y = 64;
			resetAnimation = true;
			rotation = 0;
		}
		else
		{
			rotation = 0;

			if (resetAnimation == true)
			{
				resetAnimation = false;
				elapsedTime = 0;
			}

			elapsedTime += Time::deltaTime() / 100.0f * std::abs(velocity.x);
			texturePos.x = (mirror + int(elapsedTime) % 12) * 64;
			texturePos.y = 0;


			if (texturePos.x == 5 * 64 || texturePos.x == 10 * 64)
			{
				if (stepped == false)
				{
					stepped = true;
					int channel = rand() % 4;
					Mix_Volume(channel, std::abs(velocity.x) * 2);
					Mix_PlayChannel(channel, footStep[channel], 0);
				}
			}
			else
			{
				stepped = false;
			}
		}
	}
	else
	{
		cachedVelocity = velocity.y;

		hitGround = false;

		texturePos.y = 64;
		if (velocity.y < 0)
		{
			texturePos.x = (mirror + 8) * 64;
			rotation = std::lerp(rotation, -velocity.x, Time::deltaTime() * 0.005);
		}
		else
		{
			texturePos.x = (mirror + 9) * 64;
			rotation = std::lerp(rotation, velocity.x, Time::deltaTime() * 0.005);
		}
		resetAnimation = true;
	}

	//rotation = std::lerp(rotation, -velocity.x, Time::deltaTime() * 0.05); // cool veloctiy based rotation but it doesn't look too good with pixel art

	Player::physics(phys);
}
