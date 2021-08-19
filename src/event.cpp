#include "event.hpp"

#include <SDL2/SDL.h>
#include <map>

bool quit = false;

std::map<int, bool> mouseMap;

std::map<int, bool> keyMap;

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
  		case SDL_MOUSEBUTTONDOWN:
  		{
        mouseMap[event.button.button] = true;
        break;
		  }
      break;
			case SDL_MOUSEBUTTONUP:
    	{
    		mouseMap[event.button.button] = false;
        break;
			}
      break;
      case SDL_KEYDOWN:
      {
        keyMap[event.key.keysym.sym] = true;
        break;
      }
      break;
      case SDL_KEYUP:
      {
        keyMap[event.key.keysym.sym] = false;
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

bool Event::KeyPressed(int i)
{
  return keyMap.find(i)->second; 
}

bool Event::MousePressed(int i)
{
	return mouseMap.find(i)->second;
}