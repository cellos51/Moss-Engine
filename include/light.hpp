#pragma once

#include "math.hpp"

class Light // LET THERE BE LIGHT! -idk god probably
{
public:
	Light();
	Light(Vector2 p_pos);
	int radius = 300;
	Vector2 transform;
	unsigned int layer = 1;
	float intensity = 1;
	float r = 1,g = 1,b = 1;
};