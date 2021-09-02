#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "entity.hpp"
#include "player.hpp"
#include "event.hpp"

Player::Player(Vector2 p_pos, SDL_Texture* p_tex, Vector2 p_size) : Entity(p_pos,p_tex, p_size)
{
	transform = p_pos;
	tex = p_tex;
	size = p_size;
	Player::init();
}

void Player::init() // this was the biggest pain in the fucking ass ever
{
	Entity::friction = friction;
	Entity::dragX = dragX;
	Entity::dragY = dragY;
	Entity::gravity = gravity;
	Entity::bounciness = bounciness;

	texturePos.x = 0;
	texturePos.y = 0;
	texturePos.w = size.x;
	texturePos.h = size.y;
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = size.x;
	currentFrame.h = size.y;
}

void Player::update()
{
	// SDL_GetMouseState(&mouseX, &mouseY);
	if (Event::KeyPressed(SDLK_RIGHT) || Event::KeyPressed(SDLK_d))
	{
		velocity.x += 0.0625 * Time::deltaTime();
	}

	if (Event::KeyPressed(SDLK_LEFT) || Event::KeyPressed(SDLK_a))
	{
		velocity.x += -0.0625 * Time::deltaTime();
	}

	if ((Event::KeyPressed(SDLK_UP) && OnGround == true) || (Event::KeyPressed(SDLK_w) && OnGround == true))
	{
		velocity.y = -25;
	}
	else if (!Event::KeyPressed(SDLK_UP) && !Event::KeyPressed(SDLK_w))
	{
		if (velocity.y < 0)
		{
			velocity.y += 0.1875 * Time::deltaTime();
		}
	}

	OnGround = false;
	
	Player::physics(phys);
}