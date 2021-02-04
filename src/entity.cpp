#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.hpp"

const short SCREEN_WIDTH = 1000;
const short SCREEN_HEIGHT = 600;


Entity::Entity(Vector2 p_pos, SDL_Texture* p_tex, Vector2 p_size) : transform(p_pos), tex(p_tex), size(p_size)
{
	init();
}

Entity::Entity(SDL_Texture* p_tex, Vector2 p_size) : transform(Vector2(0.0, 0.0)), tex(p_tex), size(p_size)
{
	init();
}

Entity::Entity(Vector2 p_pos, Vector2 p_size) : transform(p_pos), tex(NULL)
{
	init();
}

Entity::Entity(Vector2 p_size) : transform(Vector2(0.0, 0.0)), tex(NULL), size(p_size)
{
	init();
}

void Entity::init()
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = size.x;
	currentFrame.h = size.y;
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

Vector2 Entity::getSize()
{
	return size;
}

void Entity::physics(bool p_phys)
{
	if (p_phys == true)
	{
		velocity.x += gravity.x;
		velocity.y += gravity.y;
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

		if (getPos().y >= SCREEN_HEIGHT - currentFrame.h) // prevent leaving screen on y axis and bounces and friction :)
		{
			setY(SCREEN_HEIGHT - currentFrame.h);
			velocity.y = velocity.y * bounciness;
			if(velocity.x >= -0.5 && velocity.x <= 0.5)
			{
				velocity.x = 0;
			}
			else if (velocity.x >= 0.1)
			{
				velocity.x -= gravity.y * friction;
			}
			else if (velocity.x <= -0.1)
			{
				velocity.x += gravity.y * friction;
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
				velocity.x -= gravity.y * friction;
			}
			else if (velocity.x <= -0.1)
			{
				velocity.x += gravity.y * friction;
			}
		}

		if (getPos().x >= SCREEN_WIDTH - currentFrame.w) // prevent leaving screen on x axis and bounces and friction :)
		{
			setX(SCREEN_WIDTH - currentFrame.w);
			velocity.x = velocity.x * bounciness;
			if(velocity.y >= -0.5 && velocity.y <= 0.5)
			{
				velocity.y = 0;
			}
			else if (velocity.y >= 0.1)
			{
				velocity.y -= gravity.x * friction;
			}
			else if (velocity.y <= -0.1)
			{
				velocity.y += gravity.x * friction;
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
				velocity.y -= gravity.x * friction;
			}
			else if (velocity.y <= -0.1)
			{
				velocity.y += gravity.x * friction;
			}
		}
	}
}

void Entity::update()
{
	Entity::physics(phys);
}