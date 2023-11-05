#include "player.hpp"

#include "event.hpp"

#include <cmath>

void Player::update()
{
	elapsedTime += Time::deltaTime() / animationSpeed;

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

	//rotation = std::lerp(rotation, -velocity.x, Time::deltaTime() * 0.05); // cool veloctiy based rotation but it doesn't look too good with pixel art

	if (moveDir > 0)
	{
		texturePos.w = 64;
	}
	else if (moveDir < 0)
	{
		texturePos.w = -64;
	}
	

	texturePos.x = (int(elapsedTime) % 8) * 64;

	Player::physics(phys);
}
