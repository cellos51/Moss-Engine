#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

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

bool mouseDown = false;

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
		setVol(Vector2(getPos().x - lastFrame.x, getPos().y - lastFrame.y));
	}

	lastFrame = getPos();
}


void Player::goToMouse(bool p_tru)
{
	if (touchingMouse() && p_tru && mouseDown == false)
	{
		offset = (Vector2(mouseX - getPos().x, mouseY - getPos().y));
		priority++;
		localPriority = priority;
		mouseDown = true;
	}
	else if (p_tru == false)
	{
		mouseDown = false;
	}
}

short Player::getSize()
{
	return size;
}

bool Player::touchingMouse()
{
	if (mouseX >= getPos().x && mouseX <= getPos().x + size && mouseY >= getPos().y && mouseY <= getPos().y + size)
	{
		return true;
	}
	else
	{
		return false;
	}

}