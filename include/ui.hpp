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
	int red, green, blue;
	Text uiText;
	class Button;
	class TextInput;
	class Panel;
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
	unsigned int charLimit = 15;
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