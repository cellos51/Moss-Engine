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

void Entity::getCol(Entity& p_ent) // ok the collision is fucky as hell so only use squares with this
{
	if (p_ent.colUp == true || p_ent.colDown == true || p_ent.colLeft == true || p_ent.colRight == true)
	{
		Vector2 boxNum = Vector2((transform.x - p_ent.size.x / 2 + size.x / 2 - p_ent.transform.x) / (((size.x + p_ent.size.x) / 2 ) / 64), (transform.y - p_ent.size.y / 2 + size.y / 2 - p_ent.transform.y) / (((size.y + p_ent.size.y) / 2 ) / 64) );

		if (transform.y >= p_ent.transform.y - size.y && transform.y <= p_ent.transform.y + p_ent.size.y && transform.x >= p_ent.transform.x - size.x && transform.x <= p_ent.transform.x + p_ent.size.x)
		{	
			if (boxNum.y < 0 && fabs(boxNum.y) > fabs(boxNum.x) && p_ent.colUp == true)
			{
				OnGround = true;
				transform.y = (p_ent.transform.y - size.y);
				velocity.y = (velocity.y * -bounciness) / 16 * Time::deltaTime();

				if(velocity.x >= -0.5 / 16 * Time::deltaTime() && velocity.x <= 0.5 / 16 * Time::deltaTime())
				{
					velocity.x = 0;
				}
				else if (velocity.x > 0)
				{
					velocity.x -= (gravity.y * friction) / 16 * Time::deltaTime();
				}
				else if (velocity.x < 0)
				{
					velocity.x += (gravity.y * friction) / 16 * Time::deltaTime();
				}
			}
			else if (boxNum.y > 0 && fabs(boxNum.y) > fabs(boxNum.x) && p_ent.colDown == true)
			{
				transform.y = (p_ent.transform.y + p_ent.size.y);
				velocity.y = -velocity.y * bounciness / 16 * Time::deltaTime();

				if(velocity.x >= -0.5 / 16 * Time::deltaTime() && velocity.x <= 0.5 / 16 * Time::deltaTime())
				{
					velocity.x = 0;
				}
				else if (velocity.x > 0)
				{
					velocity.x -= (gravity.y * friction) / 16 * Time::deltaTime();
				}
				else if (velocity.x < 0)
				{
					velocity.x += (gravity.y * friction) / 16 * Time::deltaTime();
				}
			}
			else if (boxNum.x > 0 && fabs(boxNum.x) > fabs(boxNum.y) && p_ent.colRight == true)
			{
				transform.x = (p_ent.transform.x + p_ent.size.x);
				velocity.x = (-velocity.x * bounciness) / 16 * Time::deltaTime();

				if(velocity.y >= -0.5 / 16 * Time::deltaTime() && velocity.y <= 0.5 / 16 * Time::deltaTime())
				{
					velocity.y = 0;
				}
				else if (velocity.y > 0)
				{
					velocity.y -= (gravity.x * friction) / 16 * Time::deltaTime();
				}
				else if (velocity.y < 0)
				{
					velocity.y += (gravity.x * friction) / 16 * Time::deltaTime();
				}
			}
			else if (boxNum.x < 0 && fabs(boxNum.x) > fabs(boxNum.y) && p_ent.colLeft == true)
			{
				transform.x = (p_ent.transform.x - size.x);
				velocity.x = (velocity.x * -bounciness) / 16 * Time::deltaTime();
				
				if(velocity.y >= -0.5 / 16 * Time::deltaTime() && velocity.y <= 0.5 / 16 * Time::deltaTime())
				{
					velocity.y = 0;
				}
				else if (velocity.y > 0)
				{
					velocity.y -= (gravity.x * friction) / 16 * Time::deltaTime();
				}
				else if (velocity.y < 0)
				{
					velocity.y += (gravity.x * friction) / 16 * Time::deltaTime();
				}
			}
		}
	}

}

void Entity::physics(bool p_phys)
{
	if (p_phys == true)
	{
		velocity.x += gravity.x / 16 * Time::deltaTime();
		velocity.y += gravity.y / 16 * Time::deltaTime();
		transform = (Vector2(transform.x + (velocity.x / 16 * Time::deltaTime()), transform.y + (velocity.y / 16 * Time::deltaTime())));

		if(velocity.x >= -0.1 / 16 * Time::deltaTime() && velocity.x <= 0.1 / 16 * Time::deltaTime()) // air drag
		{
			velocity.x = 0;
		}
		else if (velocity.x > 0)
		{
			velocity.x -= (dragX * velocity.x) / 16 * Time::deltaTime();
		}
		else if (velocity.x < 0)
		{
			velocity.x -= (dragX * velocity.x) / 16 * Time::deltaTime();
		}

		if(velocity.y >= -0.1 / 16 * Time::deltaTime() && velocity.y <= 0.1 / 16 * Time::deltaTime())
		{
			velocity.y = 0;
		}
		else if (velocity.y > 0)
		{
			velocity.y -= (dragY * velocity.y) / 16 * Time::deltaTime();
		}
		else if (velocity.y < 0)
		{
			velocity.y -= (dragY * velocity.y) / 16 * Time::deltaTime();
		}
		
		// only enable this if you want collisions to happen on the edges of the screen (made for 60fps only)
		// if (getPos().y >= SCREEN_HEIGHT - currentFrame.h) // prevent leaving screen on y axis and bounces and friction :)
		// {
		// 	setY(SCREEN_HEIGHT - currentFrame.h);
		// 	velocity.y = velocity.y * bounciness;
		// 	if(velocity.x >= -0.5 && velocity.x <= 0.5)
		// 	{
		// 		velocity.x = 0;
		// 	}
		// 	else if (velocity.x >= 0.1)
		// 	{
		// 		velocity.x -= gravity.y * friction;
		// 	}
		// 	else if (velocity.x <= -0.1)
		// 	{
		// 		velocity.x += gravity.y * friction;
		// 	}
		// }
		// else if (getPos().y <= 0)
		// {
		// 	setY(0);
		// 	velocity.y = -velocity.y * -bounciness;
		// 	if(velocity.x >= -0.5 && velocity.x <= 0.5)
		// 	{
		// 		velocity.x = 0;
		// 	}
		// 	else if (velocity.x >= 0.1)
		// 	{
		// 		velocity.x -= gravity.y * friction;
		// 	}
		// 	else if (velocity.x <= -0.1)
		// 	{
		// 		velocity.x += gravity.y * friction;
		// 	}
		// }

		// if (getPos().x >= SCREEN_WIDTH - currentFrame.w) // prevent leaving screen on x axis and bounces and friction :)
		// {
		// 	setX(SCREEN_WIDTH - currentFrame.w);
		// 	velocity.x = velocity.x * bounciness;
		// 	if(velocity.y >= -0.5 && velocity.y <= 0.5)
		// 	{
		// 		velocity.y = 0;
		// 	}
		// 	else if (velocity.y >= 0.1)
		// 	{
		// 		velocity.y -= gravity.x * friction;
		// 	}
		// 	else if (velocity.y <= -0.1)
		// 	{
		// 		velocity.y += gravity.x * friction;
		// 	}
		// }
		// else if (getPos().x <= 0)
		// {
		// 	setX(0);
		// 	velocity.x = -velocity.x * -bounciness;
		// 	if(velocity.y >= -0.5 && velocity.y <= 0.5)
		// 	{
		// 		velocity.y = 0;
		// 	}
		// 	else if (velocity.y >= 0.1)
		// 	{
		// 		velocity.y -= gravity.x * friction;
		// 	}
		// 	else if (velocity.y <= -0.1)
		// 	{
		// 		velocity.y += gravity.x * friction;
		// 	}
		// }
	}
}

void Entity::update()
{
	Entity::physics(phys);
}