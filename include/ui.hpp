#pragma once

#include "math.hpp"
#include "text.hpp"

class ui
{
protected:
	ui();
public:
	Vector2 transform;
	Vector2 size;
	unsigned int layer;
	unsigned int tex;
	SDL_Rect offset;
	SDL_Rect texturePos;
	Color4 color;
	Color4 luminosity;
	Text uiText;
	bool touchingMouse();
	class Button;
	class TextInput;
	class Panel;
	class Slider;
};

class  ui::Button : public ui
{
public:
	using ui::ui;
	bool onClick();
};

class  ui::TextInput : public ui
{
public:
	//using ui::ui;
	TextInput();
	unsigned int charLimit = 100;
	bool startTextInput();
	~TextInput();
private:
	bool isActive = false;
};

class  ui::Panel : public ui
{
public:
	using ui::ui;
};

class  ui::Slider : public ui
{
public:
	Slider();
	using ui::ui;
	float min = 0;
	float max = 1; 
	float value = 0.5;
	bool showValue = false;
	Entity bar = Entity(Vector2(100, 100), Vector2(100, 100)); // i should probably not be using this here but i don't feel like making a whole new class for this shit y'know?
	void poll();
private:
	bool isActive = false;
};