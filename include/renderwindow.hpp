#pragma once

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "entity.hpp"
#include "ui.hpp"
#include "math.hpp"
#include "text.hpp"
#include "light.hpp"

class RenderWindow
{
public:
	void create(const char* p_title, int p_w, int p_h);
	unsigned int loadTexture(const char* p_filePath);
	void clear();
	void render(Entity& p_ent, bool cam);
	void render(Text& p_text, bool cam);
	void render(ui& p_ui);
	void render(Light& p_light);
	void display();
	void quit();
	void camera(Vector2 pos);
	void setZoom(float x);
	Vector2 getSize();
	Vector2 cameraPos = Vector2(0,0);
private:
	Vector2 cameraOffset = Vector2(0,0);
	SDL_Window* window;
	SDL_GLContext context;
	SDL_Renderer* renderer;
	
	unsigned int FBOLight;
	unsigned int FBOLightTex;

	unsigned int RBO;
	unsigned int FBOTex;
	unsigned int FBO, VAO, VBO, EBO, IVBO[5];
};