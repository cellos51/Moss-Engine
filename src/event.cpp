#include "event.hpp"

#include <SDL2/SDL.h>
#include <map>

bool quit = false;

std::map<int, bool> mouseMap;

std::map<int, bool> mouseDownMap;

std::map<int, bool> keyMap;

std::map<int, bool> keyDownMap;

void Event::PollEvent()
{
	for (auto & element : keyDownMap)
	{
    element.second = false;
	}

	for (auto & element : mouseDownMap)
	{
    element.second = false;
	}

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
        mouseDownMap[event.button.button] = true;
        break;
		  }
      break;
			case SDL_MOUSEBUTTONUP:
    	{
    		mouseMap[event.button.button] = false;
    		mouseDownMap[event.button.button] = false;
        break;
			}
      break;
      case SDL_KEYDOWN:
      {
      	if (event.key.repeat == 0)
      	{
      		keyMap[event.key.keysym.sym] = true;
        	keyDownMap[event.key.keysym.sym] = true;
      	}
        break;
      }
      break;
      case SDL_KEYUP:
      {
        if (event.key.repeat == 0)
      	{
      		keyMap[event.key.keysym.sym] = false;
        	keyDownMap[event.key.keysym.sym] = false;
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

bool Event::MousePressed(int i)
{
	return mouseMap.find(i)->second;
}

bool Event::MouseDown(int i)
{
	return mouseDownMap.find(i)->second;
}

bool Event::KeyPressed(int i)
{
  return keyMap.find(i)->second; 
}

bool Event::KeyDown(int i)
{
  return keyDownMap.find(i)->second; 
}