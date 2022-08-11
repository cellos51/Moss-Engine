#pragma once

struct Vector2
{
	Vector2();
	Vector2(float p_x, float p_y);
	Vector2 lerp(Vector2 a, Vector2 b, float time);
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
	float clamp(float value, float min, float max);
};