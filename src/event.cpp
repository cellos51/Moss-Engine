#include "event.hpp"

#include <SDL2/SDL.h>

bool quit = false;

bool mouse[3] = {0};

bool keyboard[5] = {0};

int scrollId = 0;

void Event::PollEvent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type) 
		{
    	case SDL_QUIT: 
      			quit = true;
      			break;
        case SDL_MOUSEWHEEL:
        {
          if (event.wheel.y > 0)
          {
            scrollId += 1;
            if (scrollId > 13)
            {
              scrollId = 0;
            }
          }
          else if (event.wheel.y < 0)
          {
            scrollId -= 1;
            if (scrollId < 0)
            {
              scrollId = 13;
            }
          }
        }
        break;
    		case SDL_MOUSEBUTTONDOWN:
    		{
          switch (event.button.button)
          {
      			case SDL_BUTTON_LEFT:
     					mouse[0] = true;
              break;
     				case SDL_BUTTON_RIGHT:
     					mouse[1] = true;
              break;
     				case SDL_BUTTON_MIDDLE:
     					mouse[2] = true;
              break;
          }
   				break;
			  }
        break;
			case SDL_MOUSEBUTTONUP:
    	{
    		switch (event.button.button)
        {
          case SDL_BUTTON_LEFT:
            mouse[0] = false;
            break;
          case SDL_BUTTON_RIGHT:
            mouse[1] = false;
            break;
          case SDL_BUTTON_MIDDLE:
            mouse[2] = false;
            break;
        }
   			break;		
			}
      break;
      case SDL_KEYDOWN:
      {
        switch (event.key.keysym.sym)
        {
          case SDLK_LEFT:
              keyboard[0] = true;
              break;
            case SDLK_RIGHT:
              keyboard[1] = true;
              break;
            case SDLK_UP:
              keyboard[2] = true;
              break;
            case SDLK_DOWN:
              keyboard[3] = true;
              break;
            case SDLK_s:
              keyboard[4] = true;
              break;      
        }
        break;
      }
      break;
      case SDL_KEYUP:
      {
        switch (event.key.keysym.sym)
        {
          case SDLK_LEFT:
              keyboard[0] = false;
              break;
            case SDLK_RIGHT:
              keyboard[1] = false;
              break;
            case SDLK_UP:
              keyboard[2] = false;
              break;
            case SDLK_DOWN:
              keyboard[3] = false;
              break;
            case SDLK_s:
              keyboard[4] = false;
              break;  
        }
        break;
      }
      break;
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

bool Event::KeyPressed(KeyButton i)
{
	return keyboard[i];
}

bool Event::MousePressed(MouseButton i)
{
	return mouse[i];
}

int Event::MouseWheel()
{
  return scrollId;
}