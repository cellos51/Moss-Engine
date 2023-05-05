#pragma once

struct Vector2
{
	Vector2();
	Vector2(float p_x, float p_y);
	Vector2 lerp(Vector2 a, Vector2 b, float time);
	float magnitude();
	float x, y;
};

struct Color4
{
	Color4();
	Color4(float p_r, float p_g, float p_b, float p_a);
	//Vector2 lerp(Vector2 a, Vector2 b, float time);
	float r, g, b, a;
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
	float rad2deg(float value);
};