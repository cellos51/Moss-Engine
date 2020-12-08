#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.hpp"
#include "player.hpp"

const float gravity = 1;
const float friction = 0.1;
const float drag = 0.05;
const float bounciness = -0.8;
const short size = 61;

int mouseX, mouseY;
bool mouseDown = false;
Vector2 offset;
Vector2 lastFrame = Vector2(0,0);


Player::Player(Vector2 p_pos, SDL_Texture* p_tex) : Entity{p_pos, p_tex}
{

}

Player::Player(SDL_Texture* p_tex) : Entity{Vector2(0.0, 0.0), p_tex}
{

}

Player::Player(Vector2 p_pos) :  Entity{p_pos, NULL}
{

}

Player::Player() : Entity{Vector2(0.0, 0.0), NULL}
{

}

void Player::update()
{
	velocity.y += gravity;

	Entity::transform.y += velocity.y;
	Entity::transform.x += velocity.x;

	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseDown)
	{
		Entity::transform = Vector2(mouseX - offset.x, mouseY - offset.y);

		velocity = Vector2(Entity::transform.x - lastFrame.x,Entity::transform.y - lastFrame.y);
	}

	Player::physics();

	lastFrame = Entity::transform;
}

void Player::physics()
{
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

	if (Entity::transform.y >= 597 - size) // prevent leaving screen on y axis and bounces :)
	{
		Entity::transform.y = 597 - size;
		velocity.y = velocity.y * bounciness;
		if(velocity.x >= -1 && velocity.x <= 1)
		{
			velocity.x = 0;
		}
		else if (velocity.x >= 0.1)
		{
			velocity.x -= friction * 10;
		}
		else if (velocity.x <= -0.1)
		{
			velocity.x += friction * 10;
		}
	}
	else if (Entity::transform.y <= 0)
	{
		Entity::transform.y = 0;
		velocity.y = -velocity.y * -bounciness;
		if(velocity.x >= -1 && velocity.x <= 1)
		{
			velocity.x = 0;
		}
		else if (velocity.x >= 0.1)
		{
			velocity.x -= friction * 10;
		}
		else if (velocity.x <= -0.1)
		{
			velocity.x += friction * 10;
		}
	}

	if (Entity::transform.x >= 797 - size) // prevent leaving screen on x axis and bounces :)
	{
		Entity::transform.x = 797 - size;
		velocity.x = velocity.x * bounciness;
		if(velocity.y >= -1 && velocity.y <= 1)
		{
			velocity.y = 0;
		}
		else if (velocity.y >= 0.1)
		{
			velocity.y -= friction * 10;
		}
		else if (velocity.y <= -0.1)
		{
			velocity.y += friction * 10;
		}
	}
	else if (Entity::transform.x <= 0)
	{
		Entity::transform.x = 0;
		velocity.x = -velocity.x * -bounciness;
		if(velocity.y >= -1 && velocity.y <= 1)
		{
			velocity.y = 0;
		}
		else if (velocity.y >= 0.1)
		{
			velocity.y -= friction * 10;
		}
		else if (velocity.y <= -0.1)
		{
			velocity.y += friction * 10;
		}
	}
}

void Player::goToMouse(bool p_tru)
{
	if (mouseX >= Entity::transform.x && mouseX <= Entity::transform.x + size && mouseY >= Entity::transform.y && mouseY <= Entity::transform.y + 61 && p_tru)
	{
		offset = (Vector2(mouseX - Entity::transform.x, mouseY - Entity::transform.y));
		mouseDown = true;
	}
	else
	{
		mouseDown = false;
	}
}
