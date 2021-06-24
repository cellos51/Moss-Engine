#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"
#include "entity.hpp"

class Player : public Entity
{
public:
	using Entity::Entity;
	bool phys = true;
	void init();
	void update();
	// void goToMouse(bool p_tru);
	// bool touchingMouse();
private:
	// Vector2 offset;
	// Vector2 lastFrame;
	// short localPriority;
};