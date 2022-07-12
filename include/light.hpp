#pragma once

#include "math.hpp"

class Light // LET THERE BE LIGHT! -idk god probably
{
public:
	Light();
	Light(Vector2 p_pos);
	int radius = 168;
	Vector2 transform;
	unsigned int layer = 1;
};