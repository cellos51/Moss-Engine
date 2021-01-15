#include "event.hpp"

#include <SDL2/SDL.h>

bool quit = false;

bool mouse[3];

void Event::PollEvent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type) 
		{
    		case SDL_QUIT: 
    		{
      			quit = true;
      			break;
    		}
    		case SDL_MOUSEBUTTONDOWN:
    		{
    			if (event.button.button == SDL_BUTTON_LEFT)
   				{
   					mouse[0] = true;
   				}	
   				if (event.button.button == SDL_BUTTON_RIGHT)
   				{
   					mouse[1] = true;
   				}
   				if (event.button.button == SDL_BUTTON_MIDDLE)
   				{
   					mouse[2] = true;
   				}
   				break;
			}
			case SDL_MOUSEBUTTONUP:
    		{
    			if (event.button.button == SDL_BUTTON_LEFT)
   				{
   					mouse[0] = false;
   				}
   				if (event.button.button == SDL_BUTTON_RIGHT)
   				{
   					mouse[1] = false;
   				}
   				if (event.button.button == SDL_BUTTON_MIDDLE)
   				{
   					mouse[2] = false;
   				}
   				break;		
			}
		}
	}
}

bool Event::AppQuit()
{
	if (quit)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Event::KeyPressed()
{
	return true;
}

bool Event::MousePressed(MouseButton i)
{
	return mouse[i];
}