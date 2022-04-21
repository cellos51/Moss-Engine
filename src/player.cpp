#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "physicsentity.hpp"
#include "player.hpp"
#include "event.hpp"

Player::Player(Vector2 p_pos, unsigned int p_tex, Vector2 p_size) : PhysicsEntity(p_pos,p_tex, p_size)
{
	transform = p_pos;
	tex = p_tex;
	size = p_size;
	Player::init();
}

void Player::init() // this was the biggest pain in the fucking ass ever
{
	PhysicsEntity::friction = friction;
	PhysicsEntity::dragX = dragX;
	PhysicsEntity::dragY = dragY;
	PhysicsEntity::gravity = gravity;
	PhysicsEntity::bounciness = bounciness;

	texturePos.x = 0;
	texturePos.y = 0;
	texturePos.w = size.x;
	texturePos.h = size.y;
	offset.x = 0; //offset is for textures
	offset.y = 0;
	offset.w = size.x;
	offset.h = size.y;
}

void Player::update()
{
	// SDL_GetMouseState(&mouseX, &mouseY);
	if (Event::KeyPressed(SDLK_RIGHT) || Event::KeyPressed(SDLK_d))
	{
		velocity.x += 0.015625 * Time::deltaTime();
	}

	if (Event::KeyPressed(SDLK_LEFT) || Event::KeyPressed(SDLK_a))
	{
		velocity.x += -0.015625 * Time::deltaTime();
	}

	if ((Event::KeyPressed(SDLK_UP) && OnGround == true) || (Event::KeyPressed(SDLK_w) && OnGround == true))
	{
		velocity.y = -6.25;
	}
	else if (!Event::KeyPressed(SDLK_UP) && !Event::KeyPressed(SDLK_w))
	{
		if (velocity.y < 0)
		{
			velocity.y += 0.046875 * Time::deltaTime();
		}
	}

	OnGround = false;
	
	Player::physics(phys);
}