#include "player.hpp"

#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

#include "math.h"
#include "event.hpp"

Player::Player(Vector2 p_pos, unsigned int p_tex, Vector2 p_size) : LivingEntity(p_pos,p_tex, p_size)
{
	transform = p_pos;
	tex = p_tex;
	size = p_size;

	texturePos.x = 0;// imma explain this better. texturepos is like the part of the texture the sprite uses
	texturePos.y = 0;
	texturePos.w = 40;
	texturePos.h = 40;
	offset.x = -12; //offset moves the texture in world space
	offset.y = -24;
	offset.w = 40;
	offset.h = 40;
	layer = 2;
	luminosity = Color4(1,1,1,0);

	maxHealth = 100;
	health = maxHealth;
	regenSpeed = 1;
	damage = 1;
	resitence = 1;
	attackSpeed = 1;
	movementSpeed = 1;
	critChance = 0.05;
	critDamage = 1.25;
	physDamage = 0;
	physResitence = 0;
	iceDamage = 0;
	iceResitence = 0;
	fireDamage = 0;
	fireResitence = 0;
	elecDamage = 0;
	elecResitence = 0;
	windDamage = 0;
	windResitence = 0;
	hostile = false;

	LivingEntity::init();
}

void Player::update()
{
	movementDir = Vector2(0,0);

	// SDL_GetMouseState(&mouseX, &mouseY);

	if (Event::KeyPressed(SDLK_LCTRL))
	{
		running = false;
	}
	else
	{
		running = true;
	}

	if (Event::KeyPressed(SDLK_RIGHT) || Event::KeyPressed(SDLK_d))
	{
		movementDir.x += 1;
		texturePos.x = 0;
		if (running == false)
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
		if (running == false)
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
		if (running == false)
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
		if (running == false)
		{
			texturePos.y = 120;
		}
		else
		{
			texturePos.y = 280;
		}
		
	}

	if (Event::KeyPressed(SDLK_f))
	{
		Entity test(transform, size);

		if (test.intersecting(*LivingEnts[1]))
		{
			LivingEnts[1]->velocity.x = movementDir.x * 10;
			LivingEnts[1]->velocity.y = movementDir.y * 10;
		}
	}

	animationFrame = animationFrame + (0.005) * Time::deltaTime();
	texturePos.x = ((int)animationFrame) * (40);

	if (movementDir.x == 0 && movementDir.y == 0)
	{
		animationFrame = 0.9;
	}

	LivingEntity::move(movementDir);
	
	Player::physics(phys);
}