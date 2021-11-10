#pragma once

struct Vector2
{
	Vector2();
	Vector2(float p_x, float p_y);
	float x, y;
};

namespace Time
{
	void Tick();
	double deltaTime();
};

namespace std
{
	float lerp(float a, float b, float f);
};