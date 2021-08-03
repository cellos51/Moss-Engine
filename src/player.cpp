#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.hpp"
#include "player.hpp"
#include "event.hpp"


// short priority = 0;

// bool mouseDown = false;

// int mouseX, mouseY;

void Player::update()
{
	// SDL_GetMouseState(&mouseX, &mouseY);
	if (Event::KeyPressed(SDLK_RIGHTARROW))
	{
		velocity.x += 0.0625 * Time::deltaTime();
	}

	if (Event::KeyPressed(SDLK_LEFTARROW))
	{
		velocity.x += -0.0625 * Time::deltaTime();
	}

	if (Event::KeyPressed(SDLK_UPARROW) && OnGround == true)
	{
		velocity.y = -25;

	}
	else if (!Event::KeyPressed(SDLK_UPARROW))
	{
		if (velocity.y < 0)
		{
			velocity.y += 3 * Time::deltaTime();
		}
	}


	OnGround = false;

	if (Event::KeyPressed(SDLK_DOWNARROW))
	{
		velocity.y += 1;
	}
	
	Player::physics(phys);
}