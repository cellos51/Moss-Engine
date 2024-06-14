#pragma once

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	Color4 ambientLight = Color4(0.48f, 0.54f, 0.74f, 1.0f);
	std::map<unsigned int, Vector2> TextureSize;
	Vector2 clampAmount = Vector2(0,0);
	float lerpAmount = 0;
	Vector2 grassDeform = Vector2(0, 0); // temp variable until i figure out something better
private:
	Vector2 cameraOffset = Vector2(0,0);
	SDL_Window* window;
	SDL_GLContext context;
	Vector2 screenSize = Vector2(0,0);
	
	GLuint FBOLight;
	GLuint FBOLightTex;
	GLuint FBOBlurTex;

	GLuint RBO;
	GLuint FBOTex;
	GLuint FBOBlur;
	GLuint FBO, VAO, VBO, EBO, IVBO;
};

extern OpenGLWindow window;

struct EntityGPUData
{
	glm::mat4 transform;
	glm::vec4 textureCoordinates;
	glm::vec4 luminosity;
	glm::vec4 color;
	glm::vec2 position;
	glm::vec2 size;
	unsigned int textureIndex;
	unsigned int layerIndex;
	unsigned int shaderIndex;
};

struct LightGPUData
{
	glm::mat4 transform;
	glm::mat4 emissionCenter;
	glm::vec4 color;
	glm::vec2 shape;
	unsigned int layerIndex;
};