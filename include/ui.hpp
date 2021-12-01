#pragma once

#include "math.hpp"

class ui
{
protected:
	ui();
public:
	Vector2 transform;
	Vector2 size;
	class Button;
	class Panel;
};

class  ui::Button : public ui
{
public:
	using ui::ui;
	bool onClick();
};

class  ui::Panel : public ui
{
public:
	using ui::ui;
};