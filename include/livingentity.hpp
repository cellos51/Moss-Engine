#pragma once
#include "physicsentity.hpp"
#include <vector>

#include "math.hpp"

class LivingEntity : public PhysicsEntity
{
public:
	//stats
	float maxHealth = 100;
	float health = maxHealth;
	float regenSpeed = 1;
	float damage = 1;
	float resitence = 1;
	float attackSpeed = 1;
	float movementSpeed = 1;
	float sprintMultiplier = 2;
	float critChance = 0.05f;
	float critDamage = 1.25f;
	float physDamage = 0;
	float physResitence = 0;
	float iceDamage = 0;
	float iceResitence = 0;
	float fireDamage = 0;
	float fireResitence = 0;
	float elecDamage = 0;
	float elecResitence = 0;
	float windDamage = 0;
	float windResitence = 0;
	bool running = false;
	bool hostile = false;

	Entity hitBox = Entity(Vector2(0,0), Vector2(0,0));
	Vector2 movementDir = Vector2(0, 0);

	float damageEffectTimer = 0;

	void takeDamage(int amount);

	virtual void update();
	virtual ~LivingEntity();
protected:
	using PhysicsEntity::PhysicsEntity; // i don't need to change the constructer because this class should never be called on it's own
	void init();
	void move(Vector2 moveVector);
};
