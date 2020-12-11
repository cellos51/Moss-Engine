#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"
#include "entity.hpp"

class Player : public Entity
{
public:
	Player(Vector2 p_pos, SDL_Texture* p_tex);
	Player(SDL_Texture* p_tex);
	Player(Vector2 p_pos);
	Player();
	void update();
	void physics();
	void goToMouse(bool p_tru);
	short getSize();
private:
	Vector2 offset;
	bool mouseDown = false;
	Vector2 lastFrame;
	short localPriority;
	Vector2 velocity;
};