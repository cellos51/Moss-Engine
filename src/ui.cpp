#include "ui.hpp"

#include <SDL2/SDL.h>
#include "event.hpp"
#include <iostream>
// uibase

ui::ui()
{
	transform.x = 64;
	transform.y = 64;
	size.x = 64;
	size.y = 64;
}

// button

bool ui::Button::onClick()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (Event::MousePressed(SDL_BUTTON_LEFT))
	{
		if (x >= transform.x && x <= transform.x + size.x && y >= transform.y && y <= transform.y + size.y)
		{
			return true;
		}		
	}

	return false;
}

// panel