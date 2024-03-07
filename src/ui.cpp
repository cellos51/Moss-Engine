#include "ui.hpp"

#include <SDL2/SDL.h>
#include <iostream>

#include "event.hpp"

// uibase

ui::ui()
{
	transform.x = 64;
	transform.y = 64;
	color = Color4(1, 1, 1, 1);
	luminosity = Color4(0.1, 0.1, 0.1, 0); // bro this is so fucking dumb omg but i'm unironically just gonna do this for now until i setup a seperate fbo or renderpass for unlit objects
	size.x = 64;
	size.y = 64;
	uiText.setText("");
	uiText.transform = transform;
	layer = 3;
	tex = 1;
	texturePos.x = 0;
	texturePos.y = 0;
	texturePos.w = size.x;
	texturePos.h = size.y;
	offset.x = 0;
	offset.y = 0;
	offset.w = size.x;
	offset.h = size.y;
}

bool ui::touchingMouse()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (x >= transform.x && x <= transform.x + size.x && y >= transform.y && y <= transform.y + size.y)
	{
		return true;
	}

	return false;
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
	color = Color4(0.1f, 0.1f, 0.1f, 1.0f);
	size.x = 64;
	size.y = 64;
	uiText.setText("");
	//uiText.color = {10, 10, 10};
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

// slider

ui::Slider::Slider()
{
	transform.x = 64;
	transform.y = 64;
	color = Color4(255, 255, 255, 0);
	size.x = 64;
	size.y = 64;
	uiText.setText("");
	bar.tex = -1;
	bar.color = Color4(1.0, 1.0, 1.0, 0.9);
	//uiText.color = {10, 10, 10};
}

void ui::Slider::poll()
{
	int x, y;
	SDL_GetMouseState(&x, &y);



	if (Event::MouseDown(SDL_BUTTON_LEFT))
	{
		if (x >= transform.x && x <= transform.x + size.x && y >= transform.y && y <= transform.y + size.y)
		{
			isActive = true;
		}
	}
	else if (!Event::MousePressed(SDL_BUTTON_LEFT))
	{
		isActive = false;
	}

	if (isActive == true)
	{
		value = (x - transform.x) / size.x;
		value = SDL_clamp(value, min, max);

		if (showValue == true)
		{
			uiText.setText(std::to_string(value));
		}
	}

	bar.layer = layer + 1;
	bar.transform = Vector2(transform.x, transform.y);
	bar.offset.w = (int(size.x * value) + 1) & ~1;
	bar.offset.h = size.y;
}