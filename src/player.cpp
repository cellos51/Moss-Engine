#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.hpp"
#include "player.hpp"
#include "config.hpp"
#include "event.hpp"


short priority = 0;

bool mouseDown = false;

int mouseX, mouseY;

void Player::update()
{
	// SDL_GetMouseState(&mouseX, &mouseY);
	if (Event::KeyPressed(SDLK_RIGHTARROW))
	{
		velocity.x += 1;
	}

	if (Event::KeyPressed(SDLK_LEFTARROW))
	{
		velocity.x += -1;
	}

	if (Event::KeyPressed(SDLK_UPARROW))
	{
		velocity.y = -20;
	}

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