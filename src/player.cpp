#include "player.hpp"

#include "event.hpp"
#include "Windows.h"

#include <cmath>

void Player::update()
{
	float moveDir = 0.0f;

	if (Event::KeyPressed(SDLK_d))
	{
		moveDir += speed;
	}

	if (Event::KeyPressed(SDLK_a))
	{
		moveDir -= speed;
	}

	if (Event::KeyDown(SDLK_SPACE) && OnGround)
	{
		velocity.y -= jump;
	}
	else if (!Event::KeyPressed(SDLK_SPACE) && !OnGround && velocity.y < 0)
	{
		velocity.y = std::lerp(velocity.y, 0.0f, Time::deltaTime() * 0.05);
	}

	velocity.x += moveDir * Time::deltaTime();

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

	if (std::abs(velocity.x) < 0.1)
	{
		elapsedTime += Time::deltaTime() / 200.0f;
		texturePos.x = (mirror + int(elapsedTime) % 8) * 64;
		texturePos.y = 64;
		resetAnimation = true;
	}
	else if (OnGround == true)
	{
		if (resetAnimation == true)
		{
			resetAnimation = false;
			elapsedTime = 0;
		}

		elapsedTime += Time::deltaTime() / 100.0f * std::abs(velocity.x);
		texturePos.x = (mirror + int(elapsedTime) % 12) * 64;
		texturePos.y = 0;
	}

	//rotation = std::lerp(rotation, -velocity.x, Time::deltaTime() * 0.05); // cool veloctiy based rotation but it doesn't look too good with pixel art

	Player::physics(phys);
}
