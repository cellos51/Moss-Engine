#include "physicsentity.hpp"

#include <SDL2/SDL.h>
#include <math.h>
#include <iostream>

#include "math.hpp"

void PhysicsEntity::getCol(Entity& p_ent) // ok the collision *was* fucky as hell so you dont only have to use squares with this (but it only work with rects or squares :P)
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

				if (velocity.y > 0)
				{
					velocity.y = (velocity.y * -bounciness);
				}
				

				// if(velocity.x >= -0.001 / 16 * Time::deltaTime() && velocity.x <= 0.001 / 16 * Time::deltaTime())
				// {
				// 	velocity.x = 0;
				// }
				if (velocity.x > 0)
				{
					velocity.x -= (gravity.y * friction) / 16 * Time::deltaTime();
					if (velocity.x < 0)
					{
						velocity.x = 0;
					}
				}
				else if (velocity.x < 0)
				{
					velocity.x += (gravity.y * friction) / 16 * Time::deltaTime();
					if (velocity.x > 0)
					{
						velocity.x = 0;
					}
				}
			}
			else if (boxNum.y > 0 && fabs(boxNum.y) > fabs(boxNum.x) && p_ent.colDown == true)
			{
				transform.y = (p_ent.transform.y + p_ent.size.y);
				//velocity.y = (-velocity.y * bounciness) / 16 * Time::deltaTime();

				if (velocity.y < 0)
				{
					velocity.y = (-velocity.y * bounciness);
				}

				// if(velocity.x >= -0.001 / 16 * Time::deltaTime() && velocity.x <= 0.001 / 16 * Time::deltaTime())
				// {
				// 	velocity.x = 0;
				// }
				if (velocity.x > 0)
				{
					velocity.x -= (gravity.y * friction) / 16 * Time::deltaTime();
					if (velocity.x < 0)
					{
						velocity.x = 0;
					}
				}
				else if (velocity.x < 0)
				{
					velocity.x += (gravity.y * friction) / 16 * Time::deltaTime();
					if (velocity.x > 0)
					{
						velocity.x = 0;
					}
				}
			}
			else if (boxNum.x > 0 && fabs(boxNum.x) > fabs(boxNum.y) && p_ent.colRight == true)
			{
				transform.x = (p_ent.transform.x + p_ent.size.x);
				//velocity.x = (-velocity.x * bounciness) / 16 * Time::deltaTime();

				if (velocity.x < 0)
				{
					velocity.x = (-velocity.x * bounciness);
				}

				// if(velocity.y >= -0.001 / 16 * Time::deltaTime() && velocity.y <= 0.001 / 16 * Time::deltaTime())
				// {
				// 	velocity.y = 0;
				// }
				if (velocity.y > 0)
				{
					velocity.y -= (gravity.x * friction) / 16 * Time::deltaTime();
					if (velocity.y < 0)
					{
						velocity.y = 0;
					}
				}
				else if (velocity.y < 0)
				{
					velocity.y += (gravity.x * friction) / 16 * Time::deltaTime();
					if (velocity.y > 0)
					{
						velocity.y = 0;
					}
				}
			}
			else if (boxNum.x < 0 && fabs(boxNum.x) > fabs(boxNum.y) && p_ent.colLeft == true)
			{
				transform.x = (p_ent.transform.x - size.x);
				//velocity.x = (velocity.x * -bounciness) / 16 * Time::deltaTime();

				if (velocity.x > 0)
				{
					velocity.x = (velocity.x * -bounciness);
				}
				
				// if(velocity.y >= -0.001 / 16 * Time::deltaTime() && velocity.y <= 0.001 / 16 * Time::deltaTime())
				// {
				// 	velocity.y = 0;
				// }
				if (velocity.y > 0)
				{
					velocity.y -= (gravity.x * friction) / 16 * Time::deltaTime();
					if (velocity.y < 0)
					{
						velocity.y = 0;
					}
				}
				else if (velocity.y < 0)
				{
					velocity.y += (gravity.x * friction) / 16 * Time::deltaTime();
					if (velocity.y > 0)
					{
						velocity.y = 0;
					}
				}
			}
		}
	}

}

void PhysicsEntity::physics(bool p_phys)
{
	if (p_phys == true)
	{
		OnGround = false;

		velocity.x += gravity.x / 16 * Time::deltaTime();
		velocity.y += gravity.y / 16 * Time::deltaTime();
		transform = (Vector2(transform.x + (velocity.x / 16 * Time::deltaTime()), transform.y + (velocity.y / 16 * Time::deltaTime())));

		// if(velocity.x >= -0.001 / 16 * Time::deltaTime() && velocity.x <= 0.001 / 16 * Time::deltaTime()) // air drag
		// {
		// 	velocity.x = 0;
		// }
		if (velocity.x > 0)
		{
			velocity.x -= (dragX * velocity.x) / 16 * Time::deltaTime();
			if (velocity.x < 0)
			{
				velocity.x = 0;
			}
		}
		else if (velocity.x < 0)
		{
			velocity.x -= (dragX * velocity.x) / 16 * Time::deltaTime();
			if (velocity.x > 0)
			{
				velocity.x = 0;
			}
		}

		// if(velocity.y >= -0.001 / 16 * Time::deltaTime() && velocity.y <= 0.001 / 16 * Time::deltaTime())
		// {
		// 	velocity.y = 0;
		// }
		if (velocity.y > 0)
		{
			velocity.y -= (dragY * velocity.y) / 16 * Time::deltaTime();
			if (velocity.y < 0)
			{
				velocity.y = 0;
			}
		}
		else if (velocity.y < 0)
		{
			velocity.y -= (dragY * velocity.y) / 16 * Time::deltaTime();
			if (velocity.y > 0)
			{
				velocity.y = 0;
			}
		}
	}
}