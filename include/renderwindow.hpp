#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "entity.hpp"

#include "ui.hpp"

#include "math.hpp"

#include "text.hpp"

class RenderWindow
{
public:
	void create(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);
	TTF_Font* loadFont(const char* p_filePath);
	void clear();
	void render(Entity& p_ent, bool cam);
	void render(Text& p_text, bool cam);
	void render(ui& p_ui);
	void display();
	void quit();
	void camera(Vector2 pos);
	void setRenderScale(float x, float y);
	float zoom = 0; // i have no clue what numbers work best
	Vector2 getSize();
	Vector2 cameraPos = Vector2(0,0);
private:
	Vector2 cameraOffset = Vector2(0,0);
	SDL_Window* window;
	SDL_Renderer* renderer;

};