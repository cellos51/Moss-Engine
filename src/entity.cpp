#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <iostream>

#include "entity.hpp"

#include "math.hpp"

#include "config.hpp"


Entity::Entity(Vector2 p_pos, SDL_Texture* p_tex, Vector2 p_size) : transform(p_pos), tex(p_tex), size(p_size)
{
	Entity::init();
}

Entity::Entity(SDL_Texture* p_tex, Vector2 p_size) : transform(Vector2(0.0, 0.0)), tex(p_tex), size(p_size)
{
	Entity::init();
}

Entity::Entity(Vector2 p_pos, Vector2 p_size) : transform(p_pos), tex(NULL), size(p_size)
{
	Entity::init();
}

Entity::Entity(Vector2 p_size) : transform(Vector2(0.0, 0.0)), tex(NULL), size(p_size)
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
}

SDL_Texture* Entity::getTex()
{
	return tex;
}

void Entity::setTex(SDL_Texture* p_tex)
{
	tex = p_tex;
}