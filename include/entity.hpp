#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"

class Entity
{
public:
	Entity(Vector2 p_pos, SDL_Texture* p_tex);
	Entity(SDL_Texture* p_tex);
	Entity(Vector2 p_pos);
	Entity();
	void init();
	SDL_Texture* getTex();
	void setTex(SDL_Texture* p_tex);
	SDL_Rect getCurrentFrame();
	void setPos(Vector2 p_pos);
	void setX(float p_x);
	void setY(float p_y);
	void setVolX(float p_x);
	void setVolY(float p_y);
	void setVol(Vector2 p_vol);
	Vector2 getVol();
	Vector2 getPos();
	void physics(float grav_x, float grav_y);
	void update();
private:
	Vector2 transform;
	Vector2 velocity;
	SDL_Rect currentFrame;
	SDL_Texture* tex;
};