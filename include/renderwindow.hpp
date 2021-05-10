#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "entity.hpp"

#include "math.hpp"

class RenderWindow
{
public:
	void create(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);
	void clear();
	void render(Entity& p_ent);
	void display();
	void quit();
	Vector2 camera = Vector2(0,0);
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

};