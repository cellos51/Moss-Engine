#include "networkplayer.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <cmath>

#include "math.h"
#include "event.hpp"

NetworkPlayer::NetworkPlayer(Vector2 p_pos, unsigned int p_tex, Vector2 p_size) : LivingEntity(p_pos, p_tex, p_size)
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
	luminosity = Color4(1, 1, 1, 0);

	maxHealth = 100;
	health = maxHealth;
	regenSpeed = 1;
	damage = 1;
	resitence = 1;
	attackSpeed = 1;
	movementSpeed = 1;
	critChance = 0.05f;
	critDamage = 1.25f;
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

	hitBox.layer = 10;
	hitBox.tex = window.loadTexture("assets/textures/sword.png");
	//hitBox.tex = 3;
	hitBox.luminosity = Color4(1, 1, 1, 0);
	hitBox.offset.w = 72;
	hitBox.offset.h = 72;
	hitBox.texturePos.w = 72;
	hitBox.texturePos.h = 72;

	LivingEntity::init();
}

void NetworkPlayer::update()
{
	// SDL_GetMouseState(&mouseX, &mouseY);

	if (direction == 1)
	{
		texturePos.x = 0;
		if (movement == 2)
		{
			texturePos.y = 0;
		}
		else
		{
			texturePos.y = 160;
		}
	}

	if (direction == 2)
	{
		texturePos.x = 0;
		if (movement == 2)
		{
			texturePos.y = 80;
		}
		else
		{
			texturePos.y = 240;
		}

	}

	if (direction == 3)
	{
		texturePos.x = 0;
		if (movement == 2)
		{
			texturePos.y = 40;
		}
		else
		{
			texturePos.y = 200;
		}
	}

	if (direction == 4)
	{
		texturePos.x = 0;
		if (movement == 2)
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

	if (movement == 0)
	{
		animationFrame = 0.9;
	}

	if (lastFrame != (int)animationFrame && (int)animationFrame % 2 == 0 && movement > 0)
	{
		//Mix_PlayChannel(-1, step1, 0);
	}

	lastFrame = (int)animationFrame;
}