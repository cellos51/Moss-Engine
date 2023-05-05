#include "enemy.hpp"

#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

#include "math.h"
#include "event.hpp"

Enemy::Enemy(Vector2 p_pos, unsigned int p_tex, Vector2 p_size) : LivingEntity(p_pos,p_tex, p_size)
{
	transform = p_pos;
	tex = p_tex;
	size = p_size;

	// texturePos.x = 0;// imma explain this better. texturepos is like the part of the texture the sprite uses
	// texturePos.y = 0;
	// texturePos.w = 40;
	// texturePos.h = 40;
	// offset.x = -12; //offset moves the texture in world space
	// offset.y = -24;
	// offset.w = 40;
	// offset.h = 40;
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
	hostile = true; 

	dragX = 0.3;
	dragY = 0.3;
	bounciness = 1;

	LivingEntity::init();
}

void Enemy::update()
{	
	if (damageEffectTimer > 0)
	{
		luminosity = Color4(1,0,0,0);
		damageEffectTimer -= Time::deltaTime();
	}
	else
	{
		luminosity = Color4(1,1,1,0);
	}

	Enemy::physics(phys);
}