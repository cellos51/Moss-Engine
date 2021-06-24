#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.hpp"
#include "player.hpp"
#include "event.hpp"


// short priority = 0;

// bool mouseDown = false;

// int mouseX, mouseY;

void Player::init()
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = size.x;
	currentFrame.h = size.y;
	gravity = Vector2(0.0, 1.0);
	friction = 0;
	dragX = 0.10;
	dragY = 0.01;
	bounciness = 1; // 0.8
}

void Player::update()
{
	Player::test = true;
	// SDL_GetMouseState(&mouseX, &mouseY);
	if (Event::KeyPressed(SDLK_RIGHTARROW))
	{
		velocity.x += 1;
	}

	if (Event::KeyPressed(SDLK_LEFTARROW))
	{
		velocity.x += -1;
	}

	if (Event::KeyPressed(SDLK_UPARROW) && OnGround == true)
	{
		velocity.y = -25;

	}
	else if (!Event::KeyPressed(SDLK_UPARROW))
	{
		if (velocity.y < 0)
		{
			velocity.y += 3;
		}
	}


	OnGround = false;

	if (Event::KeyPressed(SDLK_DOWNARROW))
	{
		velocity.y += 1;
	}
	
	Player::physics(phys);

	

	// if (mouseDown && localPriority == priority)
	// {
	// 	setPos(Vector2(mouseX - offset.x, mouseY - offset.y));
	// 	setVol(Vector2(getPos().x - lastFrame.x, getPos().y - lastFrame.y));
	// }

	// lastFrame = getPos();
}


// void Player::goToMouse(bool p_tru)
// {
// 	if (touchingMouse() && p_tru && mouseDown == false)
// 	{
// 		offset = (Vector2(mouseX - getPos().x, mouseY - getPos().y));
// 		priority++;
// 		localPriority = priority;
// 		mouseDown = true;
// 	}
// 	else if (p_tru == false)
// 	{
// 		mouseDown = false;
// 	}
// }

// bool Player::touchingMouse()
// {
// 	if (mouseX >= getPos().x && mouseX <= getPos().x + size.x && mouseY >= getPos().y && mouseY <= getPos().y + size.y)
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}

// }