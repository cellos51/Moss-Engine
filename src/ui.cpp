#include "ui.hpp"

#include <SDL2/SDL.h>
#include "event.hpp"
#include <iostream>
// uibase

ui::ui()
{
	transform.x = 64;
	transform.y = 64;
	red = 10;
	green = 10;
	blue = 10;
	size.x = 64;
	size.y = 64;
	uiText.setText("");
}

// button

bool ui::Button::onClick()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (Event::MouseDown(SDL_BUTTON_LEFT))
	{
		if (x >= transform.x && x <= transform.x + size.x && y >= transform.y && y <= transform.y + size.y)
		{
			return true;
		}		
	}

	return false;
}

// TextInput

ui::TextInput::TextInput()
{
	transform.x = 64;
	transform.y = 64;
	red = 255;
	green = 255;
	blue = 255;
	size.x = 64;
	size.y = 64;
	uiText.setText("");
	uiText.color = {10, 10, 10};
}

bool ui::TextInput::startTextInput()
{
	int x, y;
	SDL_GetMouseState(&x, &y);



	if (Event::MouseDown(SDL_BUTTON_LEFT))
	{
		if (x >= transform.x && x <= transform.x + size.x && y >= transform.y && y <= transform.y + size.y)
		{
			isActive = true;
			Event::TextInputEnabled(true);
			Event::SetInputText(uiText.getText());
			return true;
		}
		else
		{
			isActive = false;
			Event::TextInputEnabled(false);
			return false;
		}		
	}

	if (Event::TextInputEnabled() == true && isActive == true)
	{
		if (Event::GetInputText().size() <= charLimit)
		{
			uiText.setText(Event::GetInputText());
		}
		else if (Event::GetInputText().size() > charLimit)
		{
			Event::SetInputText(uiText.getText());
		} 
	}
	
	return false;
}

ui::TextInput::~TextInput()
{
	isActive = false;
	Event::TextInputEnabled(false);
}

// panel