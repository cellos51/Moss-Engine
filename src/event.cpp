#include "event.hpp"

#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <iostream>


bool quit = false;

bool isTextInput = false;

std::map<int, bool> mouseMap;

std::map<int, bool> mouseDownMap;

std::map<int, bool> keyMap;

std::map<int, bool> keyDownMap;

std::string text;

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
			}
			break;
			case SDL_MOUSEBUTTONUP:
			{
				mouseMap[event.button.button] = false;
				mouseDownMap[event.button.button] = false;
			}
			break;
			case SDL_KEYDOWN:
			{
				if (event.key.repeat == 0 && isTextInput == false)
				{
					keyMap[event.key.keysym.sym] = true;
					keyDownMap[event.key.keysym.sym] = true;
				}

				if( event.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0 && isTextInput == true ) //Handle backspace
				{               
					text.pop_back(); //lop off character
				}
                       
				else if( event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL && isTextInput == true ) //Handle paste
				{
					text += SDL_GetClipboardText();
				}
			}
			break;
			case SDL_KEYUP:
			{
				if (event.key.repeat == 0)
				{
					keyMap[event.key.keysym.sym] = false;
					keyDownMap[event.key.keysym.sym] = false;
				}
			}
			break;
			case SDL_TEXTINPUT: 
     		text += event.text.text; /* Add new text onto the end of our text */
			std::cout << event.text.text;
			break;
			case SDL_TEXTEDITING:
			std::cout << event.edit.text;   
			text += event.edit.text;
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
	if (mouseMap.find(i) != mouseMap.end())
	{
		return mouseMap.find(i)->second;
	}
	return false;
}

bool Event::MouseDown(int i)
{
	if (mouseDownMap.find(i) != mouseDownMap.end())
	{
		return mouseDownMap.find(i)->second;
	}
	return false;
}

bool Event::KeyPressed(int i)
{
	if (keyMap.find(i) != keyMap.end())
	{
		return keyMap.find(i)->second;
	}
	return false;
}

bool Event::TextInputEnabled(bool i)
{
	isTextInput = i;
	if (i == true)
	{
		SDL_StartTextInput();
	}
	else
	{
		SDL_StopTextInput();
	}
	return isTextInput;
}

bool Event::TextInputEnabled()
{
	return isTextInput;
}

bool Event::KeyDown(int i)
{
	if (keyDownMap.find(i) != keyDownMap.end())
	{
		return keyDownMap.find(i)->second;
	}
	return false;
}

std::string Event::GetInputText()
{
	return text;
}

void Event::SetInputText(std::string i)
{
	text = i;
	return;
}