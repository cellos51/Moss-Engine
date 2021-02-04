#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"

class Entity
{
public:
	Entity(Vector2 p_pos, SDL_Texture* p_tex, Vector2 p_size);
	Entity(SDL_Texture* p_tex, Vector2 p_size);
	Entity(Vector2 p_pos, Vector2 p_size);
	Entity(Vector2 p_size);
	Vector2 gravity = Vector2(0.0, 1.0);
	float friction = 1;
	float drag = 0.05;
	float bounciness = -0.8;
	bool phys = true;
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
	Vector2 getSize();
	void physics(bool p_phys);
	void update();
protected:
	Vector2 transform;
	SDL_Texture* tex;
	Vector2 velocity;
	Vector2 size;
	SDL_Rect currentFrame;
};