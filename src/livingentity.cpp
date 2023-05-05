#include "LivingEntity.hpp"

#include <iostream>
#include <vector>

#include "math.h"

void LivingEntity::init()
{
	
}

void LivingEntity::takeDamage(int amount)
{
	damageEffectTimer = 100; // in ms
	health -= amount;
	if (health < 0)
	{
		health = 0;
	}
}


void LivingEntity::move(Vector2 moveVector)
{
	float movementMag = sqrt((moveVector.x*moveVector.x) + (moveVector.y*moveVector.y)) + 0.00001;

	float moveSpeed = movementSpeed;

	if (running == true)
	{
		moveSpeed = movementSpeed * sprintMultiplier;
	}
	

	velocity.x += ((0.015625 * moveSpeed) * moveVector.x / movementMag) * Time::deltaTime();
	velocity.y += ((0.015625 * moveSpeed) * moveVector.y / movementMag) * Time::deltaTime();
}

void LivingEntity::update()
{
	PhysicsEntity::physics(phys);
}

LivingEntity::~LivingEntity()
{

}