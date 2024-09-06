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
	double getTimeScale();
	void setTimeScale(double scale);
};

namespace std // i have no clue why this is labled std because it's not part of the standard library but whatever it's only me who will read this code anyway
{
	//float clamp(float value, float min, float max);
	//float lerp(float a, float b, float f);
	float rad2deg(float value);
};