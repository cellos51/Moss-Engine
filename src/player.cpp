#include "player.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <cmath>

#include "math.h"
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

	texturePos.x = 0;// imma explain this better. texturepos is like the part of the texture the sprite uses
	texturePos.y = 0;
	texturePos.w = 40;
	texturePos.h = 40;
	offset.x = -12; //offset moves the texture in world space
	offset.y = -24;
	offset.w = 40;
	offset.h = 40;
	layer = 2;
}

float animationFrame = 0;

void Player::update()
{
	movementDir = Vector2(0,0);
	// SDL_GetMouseState(&mouseX, &mouseY);

	if (Event::KeyPressed(SDLK_LCTRL))
	{
		movespeed = 1;
	}
	else
	{
		movespeed = 2;
	}

	if (Event::KeyPressed(SDLK_RIGHT) || Event::KeyPressed(SDLK_d))
	{
		movementDir.x += 1;
		texturePos.x = 0;
		if (movespeed == 2)
		{
			texturePos.y = 0;
		}
		else
		{
			texturePos.y = 160;
		}
	}

	if (Event::KeyPressed(SDLK_LEFT) || Event::KeyPressed(SDLK_a))
	{
		movementDir.x += -1;
		texturePos.x = 0;
		if (movespeed == 2)
		{
			texturePos.y = 80;
		}
		else
		{
			texturePos.y = 240;
		}
		
	}

	if ((Event::KeyPressed(SDLK_UP)) || (Event::KeyPressed(SDLK_w)))
	{
		movementDir.y += -1;
		texturePos.x = 0;
		if (movespeed == 2)
		{
			texturePos.y = 40;
		}
		else
		{
			texturePos.y = 200;
		}
		
	}

	if ((Event::KeyPressed(SDLK_DOWN)) || (Event::KeyPressed(SDLK_s)))
	{
		movementDir.y += 1;
		texturePos.x = 0;
		if (movespeed == 2)
		{
			texturePos.y = 120;
		}
		else
		{
			texturePos.y = 280;
		}
		
	}

	animationFrame = animationFrame + (0.005) * Time::deltaTime();
	texturePos.x = ((int)animationFrame) * (40);

	if (movementDir.x == 0 && movementDir.y == 0)
	{
		animationFrame = 0.9;
	}
	
	float movementMag = sqrt((movementDir.x*movementDir.x) + (movementDir.y*movementDir.y)) + 0.00001;

	velocity.x += ((0.015625 * movespeed) * movementDir.x / movementMag) * Time::deltaTime();
	velocity.y += ((0.015625 * movespeed) * movementDir.y / movementMag) * Time::deltaTime();

	OnGround = false;
	
	Player::physics(phys);
}