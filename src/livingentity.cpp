#include "LivingEntity.hpp"

#include <iostream>
#include <vector>

#include "math.h"

void LivingEntity::init()
{

}

LivingEntity::~LivingEntity()
{

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