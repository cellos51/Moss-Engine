#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.hpp"

Entity::Entity(Vector2 p_pos, SDL_Texture* p_tex) : transform(p_pos), tex(p_tex)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 16;
	currentFrame.h = 16;
}

Entity::Entity(SDL_Texture* p_tex) : transform(Vector2(0.0, 0.0)), tex(p_tex)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 16;
	currentFrame.h = 16;
}

Entity::Entity(Vector2 p_pos) : transform(p_pos), tex(NULL)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 16;
	currentFrame.h = 16;
}

Entity::Entity() : transform(Vector2(0.0, 0.0)), tex(NULL)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 16;
	currentFrame.h = 16;
}

SDL_Texture* Entity::getTex()
{
	return tex;
}

void Entity::setTex(SDL_Texture* p_tex)
{
	tex = p_tex;
}

SDL_Rect Entity::getCurrentFrame()
{
	return currentFrame;
}

void Entity::setPos(Vector2 p_pos)
{
	transform = p_pos;
}

void Entity::setX(float p_x)
{
	transform.x = p_x;
}

void Entity::setY(float p_y)
{
	transform.y = p_y;
}

Vector2 Entity::getPos()
{
	return transform;
}
