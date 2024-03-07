#include "event.hpp"
#include "console.hpp"

#include <SDL2/SDL.h>
#include <map>
#include <string>
#include <iostream>


bool quit = false;

bool isTextInput = false;

bool textSubmit = false;

float mouseWheel = 0;

std::map<int, bool> mouseMap;

std::map<int, bool> mouseDownMap;

std::map<int, bool> keyMap;

std::map<int, bool> keyDownMap;

std::map<int, bool> buttonMap;

std::map<int, bool> buttonDownMap;

Vector2 leftJoy = Vector2(0.0f, 0.0f);
Vector2 rightJoy = Vector2(0.0f, 0.0f);

std::string text;

const int CONTROLLER_DEAD_ZONE = 8000;

SDL_GameController* controller = nullptr;

static SDL_GameController* findController()
{
	for (int i = 0; i < 4; i++)
	{
		if (SDL_IsGameController(i))
		{
			return SDL_GameControllerOpen(i);
		}
	}

	return nullptr;
}

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

	for (auto& element : buttonDownMap)
	{
		element.second = false;
	}

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		mouseWheel = 0;
		switch (event.type) 
		{
			case SDL_QUIT: 
			quit = true;
			break;
			case SDL_MOUSEWHEEL:
			{
				mouseWheel = event.wheel.y;
			}
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

				if (event.key.keysym.sym == SDLK_RETURN && isTextInput == true) //Handle backspace
				{
					textSubmit = true;
					TextInputEnabled(false);	
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
			case SDL_CONTROLLERDEVICEADDED:
			{
				if (!controller)
				{
					controller = SDL_GameControllerOpen(event.cdevice.which);
				}
				break;
			}
			break;
			case SDL_CONTROLLERDEVICEREMOVED:
			{
				if (controller && event.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller)))
				{
					SDL_GameControllerClose(controller);
					controller = findController();
				}
				break;
			}
			case SDL_CONTROLLERBUTTONDOWN:
			{
				buttonMap[event.cbutton.button] = true;
				buttonDownMap[event.cbutton.button] = true;
			}
			break;
			case SDL_CONTROLLERBUTTONUP:
			{
				buttonMap[event.cbutton.button] = false;
				buttonDownMap[event.cbutton.button] = false;
			}
			break;
			case SDL_CONTROLLERAXISMOTION:
			{
				switch (event.caxis.axis)
				{
				case SDL_CONTROLLER_AXIS_LEFTY:
					if (event.caxis.value < -CONTROLLER_DEAD_ZONE)
					{
						leftJoy.y = -event.caxis.value / 32767.0f;
					}
					else if (event.caxis.value > CONTROLLER_DEAD_ZONE)
					{
						leftJoy.y = -event.caxis.value / 32767.0f;
					}
					else
					{
						leftJoy.y = 0;
					}
					break;
				case SDL_CONTROLLER_AXIS_LEFTX:
					if (event.caxis.value < -CONTROLLER_DEAD_ZONE)
					{
						leftJoy.x = -event.caxis.value / 32767.0f;
					}
					else if (event.caxis.value > CONTROLLER_DEAD_ZONE)
					{
						leftJoy.x = -event.caxis.value / 32767.0f;
					}
					else
					{
						leftJoy.x = 0;
					}
					break;
				}
			}
			break;
		}
	}
}

void Event::AppStart()
{
	controller = findController();
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

bool Event::ButtonPressed(int i)
{
	if (buttonMap.find(i) != buttonMap.end())
	{
		return buttonMap.find(i)->second;
	}
	return false;
}

bool Event::ButtonDown(int i)
{
	if (buttonDownMap.find(i) != buttonDownMap.end())
	{
		return buttonDownMap.find(i)->second;
	}
	return false;
}

float Event::JoyAxis(int i)
{
	if (i == SDL_CONTROLLER_AXIS_LEFTY)
	{
		return leftJoy.y;
	}
	else if (i == SDL_CONTROLLER_AXIS_LEFTX)
	{
		return leftJoy.x;
	}
	else if (i == SDL_CONTROLLER_AXIS_RIGHTY)
	{
		return rightJoy.y;
	}
	else if (i == SDL_CONTROLLER_AXIS_RIGHTX)
	{
		return rightJoy.x;
	}

	return 0.0f;
}

float Event::MouseWheel()
{
	return mouseWheel;
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

bool Event::TextSubmitted()
{
	if (textSubmit)
	{
		textSubmit = false;
		return true;
	}

	return false;
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