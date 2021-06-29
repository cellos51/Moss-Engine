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
	void render(Entity& p_ent, bool cam);
	void render(Entity& p_ent, int offx, int offy, bool cam);
	void drawLine(int x1, int y1, int x2, int y2, bool cam);
	void display();
	void quit();
	void camera(Entity& p_ent);
	Vector2 cameraPos = Vector2(0,0);
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};