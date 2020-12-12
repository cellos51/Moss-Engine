#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.hpp"
#include "player.hpp"

const short SCREEN_WIDTH = 1000;
const short SCREEN_HEIGHT = 600;

const float gravity = 1;
const float friction = 1;
const float drag = 0.05;
const float bounciness = -0.8;
const short size = 64;

short priority = 0;

int mouseX, mouseY;

Player::Player(Vector2 p_pos, SDL_Texture* p_tex) : Entity{p_pos, p_tex} {}

Player::Player(SDL_Texture* p_tex) : Entity{Vector2(0.0, 0.0), p_tex} {}

Player::Player(Vector2 p_pos) :  Entity{p_pos, NULL} {}

Player::Player() : Entity{Vector2(0.0, 0.0), NULL} {}

void Player::update()
{
	SDL_GetMouseState(&mouseX, &mouseY);

	Player::physics(0, gravity);

	if (mouseDown && localPriority == priority)
	{
		setPos(Vector2(mouseX - offset.x, mouseY - offset.y));
		velocity = Vector2(getPos().x - lastFrame.x, getPos().y - lastFrame.y);
	}

	lastFrame = getPos();
}

void Player::physics(float grav_x, float grav_y)
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
		if(velocity.x >= -1 && velocity.x <= 1)
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
		if(velocity.x >= -1 && velocity.x <= 1)
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
		if(velocity.y >= -1 && velocity.y <= 1)
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
		if(velocity.y >= -1 && velocity.y <= 1)
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

void Player::goToMouse(bool p_tru)
{
	if (mouseX >= getPos().x && mouseX <= getPos().x + size && mouseY >= getPos().y && mouseY <= getPos().y + size && p_tru)
	{
		offset = (Vector2(mouseX - getPos().x, mouseY - getPos().y));
		priority++;
		localPriority = priority;
		mouseDown = true;
	}
	else
	{
		mouseDown = false;
	}
}

short Player::getSize()
{
	return size;
}