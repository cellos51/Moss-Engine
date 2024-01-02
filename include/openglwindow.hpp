#pragma once

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <map>

#include "entity.hpp"
#include "ui.hpp"
#include "math.hpp"
#include "text.hpp"
#include "light.hpp"

class OpenGLWindow
{
public:
	void create(const char* p_title, int p_w, int p_h);
	unsigned int loadTexture(const char* p_filePath);
	unsigned int replaceTexture(const char* p_filePath, unsigned int texture);
	void clear();
	void render(Entity& p_ent, bool cam);
	void render(Text& p_text, bool cam);
	void render(ui::Slider& p_slider);
	void render(ui& p_ui);	
	void render(Light& p_light);
	void display();
	void quit();
	void camera(Vector2 pos);
	void setZoom(float x);
	Vector2 getSize();
	Vector2 cameraPos = Vector2(0,0);
	Color4 ambientLight = Color4(0.5f, 0.5f, 0.5f,1);
	std::map<unsigned int, Vector2> TextureSize;
	Vector2 clampAmount = Vector2(0,0);
	float lerpAmount = 0;
private:
	Vector2 cameraOffset = Vector2(0,0);
	SDL_Window* window;
	SDL_GLContext context;
	//SDL_Renderer* renderer;
	Vector2 screenSize = Vector2(0,0);
	
	unsigned int FBOLight;
	unsigned int FBOLightTex;
	unsigned int FBOBlurTex;

	unsigned int RBO;
	unsigned int FBOTex;
	unsigned int FBOBlur;
	unsigned int FBO, VAO, VBO, EBO, IVBO[7];
};

extern OpenGLWindow window;