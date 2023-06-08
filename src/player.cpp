#include "player.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <cmath>

#include "math.h"
#include "event.hpp"

bool hasHit = false;

//erm audio testtt
Mix_Chunk *step1 = NULL;
Mix_Chunk *hit = NULL;

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

	hitBox.layer = 10;
	hitBox.tex = window.loadTexture( "assets/textures/sword.png" );
	//hitBox.tex = 3;
	hitBox.luminosity = Color4(1,1,1,0);
	hitBox.offset.w = 72;
	hitBox.offset.h = 72;
	hitBox.texturePos.w = 72;
	hitBox.texturePos.h = 72;

	step1 = Mix_LoadWAV( "assets/audio/step1.wav" );
	hit = Mix_LoadWAV( "assets/audio/hitsound.wav" );

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
		hitDir = Vector2(1,0);
		hitBox.rotation = 90.0f;
		hitBox.layer = layer + 1;

		movementDir.x += 1;
		texturePos.x = 0;
		if (running == true)
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
		hitDir = Vector2(-1,0);
		hitBox.rotation = -90.0f;
		hitBox.layer = layer + 1;

		movementDir.x += -1;
		texturePos.x = 0;
		if (running == true)
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
		hitDir = Vector2(0,-1);
		hitBox.rotation = 180.0f;
		hitBox.layer = layer - 1;

		movementDir.y += -1;
		texturePos.x = 0;
		if (running == true)
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
		hitDir = Vector2(0,1);
		hitBox.rotation = 0.0f;
		hitBox.layer = layer + 1;

		movementDir.y += 1;
		texturePos.x = 0;
		if (running == true)
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

	LivingEntity::move(movementDir);
	
	Player::physics(phys);

	//hitBox.transform = transform;

	if (lastFrame != (int)animationFrame && (int)animationFrame % 2 == 0 && movementDir.magnitude() > 0)
	{
		Mix_PlayChannel( -1, step1, 0 );
	}

	if (animationFrameSword > 0)
	{
		animationFrameSword = animationFrameSword - (0.025) * Time::deltaTime();
		hitBox.texturePos.x = -((int)animationFrameSword) * (72);

		if ((int)animationFrameSword == 6 && hasHit == false)
		{
			for (auto& [key, ent]: LivingEnts)
			{
				if (ent->hostile == true && hitBox.intersecting(*ent))
				{
					ent->takeDamage(0);
					ent->velocity.x = hitDir.x * 10;
					ent->velocity.y = hitDir.y * 10;
					hasHit = true;
					Mix_PlayChannel( -1, hit, 0 );
				}
			}
		}
	}

	hitBox.transform.x = transform.x - (hitBox.size.x / 2) + 8 + (hitDir.x * 6);
	hitBox.transform.y = transform.y - (hitBox.size.y / 2) + (hitDir.y * 6);

	hitBox.size = Vector2(24 + (std::abs( hitDir.y * 12 )),24 + (std::abs( hitDir.x * 12 )));


	if (hitDir.x > 0)
	{
		hitBox.offset.x = -hitBox.size.y + 4;
		hitBox.offset.y = -hitBox.size.x + 8;
	}
	else if (hitDir.x < 0)
	{
		hitBox.offset.x = -hitBox.size.y + 19;
		hitBox.offset.y = -hitBox.size.x + 5;
	}
	else if (hitDir.y > 0)
	{
		hitBox.offset.x = -hitBox.size.y + 4;
		hitBox.offset.y = -hitBox.size.x + 2;
	}
	else if (hitDir.y < 0)
	{
		hitBox.offset.x = -hitBox.size.y + 4;
		hitBox.offset.y = -hitBox.size.x + 8;
	}




	if (Event::KeyDown(SDLK_f))
	{
		hasHit = false;
		animationFrameSword = 9;

		//hitBox.size = Vector2(48,24);
	}

	lastFrame = (int)animationFrame;
}