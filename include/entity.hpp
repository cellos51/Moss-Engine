#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "math.hpp"

class Entity
{
public:
	Vector2 transform;
	Entity(Vector2 p_pos, SDL_Texture* p_tex);
	Entity(SDL_Texture* p_tex);
	Entity(Vector2 p_pos);
	Entity();
	SDL_Texture* getTex();
	void setTex(SDL_Texture* p_tex);
	SDL_Rect getCurrentFrame();
	void setY(float p_y);
private:
	SDL_Rect currentFrame;
	SDL_Texture* tex;
};