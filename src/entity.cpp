#include "entity.hpp"

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

#include "math.hpp"

Entity::Entity(Vector2 p_pos, unsigned int p_tex, Vector2 p_size) : transform(p_pos), tex(p_tex), size(p_size)
{
	Entity::init();
}

Entity::Entity(unsigned int p_tex, Vector2 p_size) : transform(Vector2(0.0, 0.0)), tex(p_tex), size(p_size)
{
	Entity::init();
}

Entity::Entity(Vector2 p_pos, Vector2 p_size) : transform(p_pos), tex(-1), size(p_size)
{
	Entity::init();
}

Entity::Entity(Vector2 p_size) : transform(Vector2(0.0, 0.0)), tex(-1), size(p_size)
{
	Entity::init();
}

void Entity::init()
{
	texturePos.x = 0;
	texturePos.y = 0;
	texturePos.w = size.x;
	texturePos.h = size.y;
	offset.x = 0;
	offset.y = 0;
	offset.w = size.x;
	offset.h = size.y;
	layer = 0;
}

unsigned int Entity::getTex()
{
	return tex;
}

void Entity::setTex(unsigned int p_tex)
{
	tex = p_tex;
}

bool Entity::intersecting(Entity p_ent)
{
	if (transform.y >= p_ent.transform.y - size.y && transform.y <= p_ent.transform.y + p_ent.size.y && transform.x >= p_ent.transform.x - size.x && transform.x <= p_ent.transform.x + p_ent.size.x)
	{
		return true;
	}

	return false;
}