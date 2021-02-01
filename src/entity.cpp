#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.hpp"

const short SCREEN_WIDTH = 1000;
const short SCREEN_HEIGHT = 600;

const float gravity = 1;
const float friction = 1;
const float drag = 0.05;
const float bounciness = -0.8;
const short size = 64;

Entity::Entity(Vector2 p_pos, SDL_Texture* p_tex) : transform(p_pos), tex(p_tex)
{
	init();
}

Entity::Entity(SDL_Texture* p_tex) : transform(Vector2(0.0, 0.0)), tex(p_tex)
{
	init();
}

Entity::Entity(Vector2 p_pos) : transform(p_pos), tex(NULL)
{
	init();
}

Entity::Entity() : transform(Vector2(0.0, 0.0)), tex(NULL)
{
	init();
}

void Entity::init()
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

void Entity::setVolX(float p_x)
{
	velocity.x = p_x;
}

void Entity::setVolY(float p_y)
{
	velocity.y = p_y;
}

void Entity::setVol(Vector2 p_vol)
{
	velocity = p_vol;
}

Vector2 Entity::getVol()
{
	return velocity;
}

Vector2 Entity::getPos()
{
	return transform;
}

void Entity::physics(float grav_x, float grav_y)
{
	velocity.x += grav_x;
	velocity.y += grav_y;
	setPos(Vector2(getPos().x + velocity.x, getPos().y + velocity.y));

	if(velocity.x >= -0.1 && velocity.x <= 0.1) // air drag
	{
		velocity.x = 0;
	}
	else if (velocity.x >= 0.1)
	{
		velocity.x -= drag;
	}
	else if (velocity.x <= -0.1)
	{
		velocity.x += drag;
	}

	if(velocity.y >= -0.1 && velocity.y <= 0.1)
	{
		velocity.y = 0;
	}
	else if (velocity.y >= 0.1)
	{
		velocity.y -= drag;
	}
	else if (velocity.y <= -0.1)
	{
		velocity.y += drag;
	}

	if (getPos().y >= SCREEN_HEIGHT - size) // prevent leaving screen on y axis and bounces and friction :)
	{
		setY(SCREEN_HEIGHT - size);
		velocity.y = velocity.y * bounciness;
		if(velocity.x >= -0.5 && velocity.x <= 0.5)
		{
			velocity.x = 0;
		}
		else if (velocity.x >= 0.1)
		{
			velocity.x -= grav_y * friction;
		}
		else if (velocity.x <= -0.1)
		{
			velocity.x += grav_y * friction;
		}
	}
	else if (getPos().y <= 0)
	{
		setY(0);
		velocity.y = -velocity.y * -bounciness;
		if(velocity.x >= -0.5 && velocity.x <= 0.5)
		{
			velocity.x = 0;
		}
		else if (velocity.x >= 0.1)
		{
			velocity.x -= grav_y * friction;
		}
		else if (velocity.x <= -0.1)
		{
			velocity.x += grav_y * friction;
		}
	}

	if (getPos().x >= SCREEN_WIDTH - size) // prevent leaving screen on x axis and bounces and friction :)
	{
		setX(SCREEN_WIDTH - size);
		velocity.x = velocity.x * bounciness;
		if(velocity.y >= -0.5 && velocity.y <= 0.5)
		{
			velocity.y = 0;
		}
		else if (velocity.y >= 0.1)
		{
			velocity.y -= grav_x * friction;
		}
		else if (velocity.y <= -0.1)
		{
			velocity.y += grav_x * friction;
		}
	}
	else if (getPos().x <= 0)
	{
		setX(0);
		velocity.x = -velocity.x * -bounciness;
		if(velocity.y >= -0.5 && velocity.y <= 0.5)
		{
			velocity.y = 0;
		}
		else if (velocity.y >= 0.1)
		{
			velocity.y -= grav_x * friction;
		}
		else if (velocity.y <= -0.1)
		{
			velocity.y += grav_x * friction;
		}
	}
}

void Entity::update()
{
	Entity::physics(0, gravity);
}