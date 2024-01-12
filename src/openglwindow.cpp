#include "openglwindow.hpp"

#include <SDL2/SDL.h>
#include <stb_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "math.hpp"
#include "entity.hpp"
#include "text.hpp"
#include "light.hpp"
#include "console.hpp"

OpenGLWindow window;

float zoom = 1;
Entity OnscreenCamera(Vector2(0,0));

Shader framebufferShader("assets/shaders/framebuffer.vert", "assets/shaders/framebuffer.frag");
Shader defaultShader("assets/shaders/shader.vert", "assets/shaders/shader.frag");
Shader shadowShader("assets/shaders/shadowshader.vert", "assets/shaders/shadowshader.frag");
Shader lightShader("assets/shaders/lightshader.vert", "assets/shaders/lightshader.frag");
Shader luminosityShader("assets/shaders/luminosity.vert", "assets/shaders/luminosity.frag");

int textureUnits[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

unsigned int newestTexture = 0;

const unsigned int maxEntities = 16383; // begin stuff for entities

int entityCount = 0; 

EntityGPUData entityData[maxEntities];

const unsigned int maxLights = 1023; // begin stuff for lights

int lightCount = 0; 

LightGPUData lightData[maxEntities];

std::map<unsigned int,std::vector<int>> TexturesToRender; // this does NOTHING but i see where this dude was coming from. ( past me made this :O )

void OpenGLWindow::create(const char* p_title, int p_w, int p_h)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetSwapInterval(0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	context = SDL_GL_CreateContext(window);
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	glViewport(0, 0, getSize().x, getSize().y);

	float vertices[] = {
    // positions    // colors  // texture coords
     1,  1, 0.0f,   1, 1, 1,   1.0f, 1.0f,   // top right
     1, -1, 0.0f,   1, 1, 1,   1.0f, 0.0f,   // bottom right
    -1, -1, 0.0f,   1, 1, 1,   0.0f, 0.0f,   // bottom left
    -1,  1, 0.0f,   1, 1, 1,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = { 
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
	};  

	framebufferShader.compile();
	defaultShader.compile();
	shadowShader.compile();
	lightShader.compile();
	luminosityShader.compile();

	int glConsts;
	
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &glConsts);
	console.log("Max Uniform Block Size: " + std::to_string(glConsts) + "\n");
	
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &glConsts);
	console.log("Max Vertex Unifrom Components: " + std::to_string(glConsts) + "\n");

	glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &glConsts);
	console.log("Max Vertex Output Components: " + std::to_string(glConsts) + "\n");

	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &glConsts);
	console.log("Max Fragment Unifrom Components: " + std::to_string(glConsts) + "\n");

	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &glConsts);
	console.log("Max Unifrom Components: " + std::to_string(glConsts) + "\n");

	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &glConsts);
	console.log("Max Array Texture Layers: " + std::to_string(glConsts) + "\n");

	glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

	glEnable(GL_STENCIL_TEST);
    glStencilFunc( GL_ALWAYS, 1, 0xFF );
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    glEnable(GL_DEPTH_TEST);
   	glDepthFunc(GL_LESS);
   	glDepthMask(GL_TRUE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenFramebuffers(1, &FBO);
	glGenFramebuffers(1, &FBOLight);
	glGenFramebuffers(1, &FBOBlur);
	glGenTextures(1, &FBOTex);
	glGenTextures(1, &FBOLightTex);
	glGenTextures(1, &FBOBlurTex);
	glGenRenderbuffers(1, &RBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(7, IVBO); 

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glActiveTexture(GL_TEXTURE0 + FBOTex);
    glBindTexture(GL_TEXTURE_2D, FBOTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_w, p_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTex, 0); 
	
	glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);  
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, p_w, p_h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		console.log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, FBOLight);

    glActiveTexture(GL_TEXTURE0 + FBOLightTex);
    glBindTexture(GL_TEXTURE_2D, FBOLightTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_w, p_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOLightTex, 0); 
	
	glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, p_w, p_h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);  

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		console.log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, FBOBlur);

	glActiveTexture(GL_TEXTURE0 + FBOBlurTex);
	glBindTexture(GL_TEXTURE_2D, FBOBlurTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_w, p_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOBlurTex, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	// texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);  

	// shadow attribute
	glVertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, GL_FALSE, (void*)(8 * sizeof(bool)));
	glEnableVertexAttribArray(3);  

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    screenSize = getSize();

	console.log("Error check: " + std::to_string(glGetError()) + "\n");
}

unsigned int OpenGLWindow::loadTexture(const char* p_filePath) // used load textures :P
{
	int width, height, nrChannels;
	unsigned int texture;
	unsigned char *data;

	stbi_set_flip_vertically_on_load(true); 

	data = stbi_load(p_filePath, &width, &height, &nrChannels, 0); 


	glGenTextures(1, &texture);  
	glActiveTexture(GL_TEXTURE0 + texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (data)
	{
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
	    return -1;
	}
	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	TextureSize.insert(std::pair<unsigned int,Vector2>(texture, Vector2(width, height)));

	//std::cout << "TextureID: " << texture << std::endl;

	newestTexture = texture;
	return texture;
}

unsigned int OpenGLWindow::replaceTexture(const char* p_filePath, unsigned int texture) // used replace textures >:(
{
	int width, height, nrChannels;
	//unsigned int texture;
	unsigned char* data;

	stbi_set_flip_vertically_on_load(true);

	data = stbi_load(p_filePath, &width, &height, &nrChannels, 0);


	//glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0 + texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		return -1;
	}
	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	TextureSize.insert(std::pair<unsigned int, Vector2>(texture, Vector2(width, height)));

	//std::cout << "TextureID: " << texture << std::endl;

	newestTexture = texture;
	return texture;
}

void OpenGLWindow::clear() // clears the renderer
{
	entityCount = 0;
	lightCount = 0;

	if (screenSize.x != getSize().x || screenSize.y != getSize().y)
	{
		screenSize = getSize();

		glViewport(0, 0, getSize().x, getSize().y);

		glBindTexture(GL_TEXTURE_2D, FBOTex);	  
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getSize().x, getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, FBOLightTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getSize().x, getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, FBOBlurTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getSize().x, getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, RBO);  
	    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, getSize().x, getSize().y);
	    glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//glClearColor(0.439, 0.502, 0.565, 1.0);
	glClearColor(ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, FBOLight);
	//glClearColor(ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);
	glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, FBOBlurTex);
	//glClearColor(ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);
	glClearColor(0, 0, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	OnscreenCamera.transform = Vector2(cameraPos.x - OnscreenCamera.size.x / 2, cameraPos.y - OnscreenCamera.size.y / 2);
	OnscreenCamera.size = Vector2(getSize().x / zoom, getSize().y / zoom);
}

void OpenGLWindow::render(Entity& p_ent, bool cam) // i think this copies the texture to the renderer
{
	if (p_ent.intersecting(OnscreenCamera) == true && cam == true && entityCount < maxEntities)
	{
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::scale(transform, glm::vec3((p_ent.offset.w) / getSize().x, (p_ent.offset.h) / getSize().y, 0));  
		transform = glm::translate(transform, glm::vec3(((((p_ent.offset.x + round(p_ent.transform.x)) + p_ent.offset.w / 2) - getSize().x / 2) - cameraOffset.x) / (p_ent.offset.w / 2), -((((p_ent.offset.y + round(p_ent.transform.y)) + p_ent.offset.h / 2) - getSize().y / 2) - cameraOffset.y) / (p_ent.offset.h / 2), 0)); 
		transform = glm::rotate(transform, std::rad2deg(p_ent.rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		entityData[entityCount].transform = transform;
		entityData[entityCount].textureCoordinates = glm::vec4(p_ent.texturePos.x / TextureSize[p_ent.tex].x,p_ent.texturePos.y / TextureSize[p_ent.tex].y, TextureSize[p_ent.tex].x / p_ent.texturePos.w, TextureSize[p_ent.tex].y / p_ent.texturePos.h);
		entityData[entityCount].textureIndex = p_ent.tex;
		entityData[entityCount].layerIndex = p_ent.layer;
		entityData[entityCount].luminosity = glm::vec4(p_ent.luminosity.r,p_ent.luminosity.g,p_ent.luminosity.b,p_ent.luminosity.a);
		entityData[entityCount].color = glm::vec4(p_ent.color.r, p_ent.color.g, p_ent.color.b, p_ent.color.a);

		if (TexturesToRender.find(p_ent.layer) == TexturesToRender.end())
		{
			std::vector<int> newVector;
			TexturesToRender.insert(std::pair<unsigned int,std::vector<int>>(p_ent.layer, newVector));
		}

		if (std::find(TexturesToRender[p_ent.layer].begin(), TexturesToRender[p_ent.layer].end(), p_ent.tex) == TexturesToRender[p_ent.layer].end())
		{
			TexturesToRender[p_ent.layer].push_back(p_ent.tex);
		}


		entityCount++;
	}
	else if (cam == false && entityCount < maxEntities)
	{
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::scale(transform, glm::vec3((p_ent.offset.w) / getSize().x, (p_ent.offset.h) / getSize().y, 0));  
		transform = glm::translate(transform, glm::vec3(((((p_ent.offset.x + round(p_ent.transform.x)) + p_ent.offset.w / 2) - getSize().x / 2)) / (p_ent.offset.w / 2), -((((p_ent.offset.y + round(p_ent.transform.y)) + p_ent.offset.h / 2) - getSize().y / 2)) / (p_ent.offset.h / 2), 0));
		transform = glm::rotate(transform, std::rad2deg(p_ent.rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		entityData[entityCount].transform = transform;
		entityData[entityCount].textureCoordinates = glm::vec4(p_ent.texturePos.x / TextureSize[p_ent.tex].x, p_ent.texturePos.y / TextureSize[p_ent.tex].y, TextureSize[p_ent.tex].x / p_ent.texturePos.w, TextureSize[p_ent.tex].y / p_ent.texturePos.h);
		entityData[entityCount].textureIndex = p_ent.tex;
		entityData[entityCount].layerIndex = p_ent.layer;
		entityData[entityCount].luminosity = glm::vec4(p_ent.luminosity.r, p_ent.luminosity.g, p_ent.luminosity.b, p_ent.luminosity.a);
		entityData[entityCount].color = glm::vec4(p_ent.color.r, p_ent.color.g, p_ent.color.b, p_ent.color.a);

		if (TexturesToRender.find(p_ent.layer) == TexturesToRender.end())
		{
			std::vector<int> newVector;
			TexturesToRender.insert(std::pair<unsigned int,std::vector<int>>(p_ent.layer, newVector));
		}

		if (std::find(TexturesToRender[p_ent.layer].begin(), TexturesToRender[p_ent.layer].end(), p_ent.tex) == TexturesToRender[p_ent.layer].end())
		{
			TexturesToRender[p_ent.layer].push_back(p_ent.tex);
		}


		entityCount++;
	}
}

void OpenGLWindow::render(Text& p_text, bool cam) // i think this copys the texture to the renderer
{
	if (p_text.getText().size() > 0)
	{
		for (Entity character : p_text.characters)
		{
			render(character, cam);
		}
	}	
}

void OpenGLWindow::render(ui::Slider& p_ui) // i have no fucking clue if this will work the way i think it will :)
{
	glm::mat4 transform = glm::mat4(1.0f);

	transform = glm::scale(transform, glm::vec3((p_ui.size.x) / getSize().x, (p_ui.size.y) / getSize().y, 0));
	transform = glm::translate(transform, glm::vec3(((((p_ui.transform.x) + p_ui.size.x / 2) - getSize().x / 2)) / (p_ui.size.x / 2), -((((p_ui.transform.y) + p_ui.size.y / 2) - getSize().y / 2)) / (p_ui.size.y / 2), 0));

	entityData[entityCount].transform = transform;
	entityData[entityCount].textureCoordinates = glm::vec4(p_ui.texturePos.x / TextureSize[p_ui.tex].x, p_ui.texturePos.y / TextureSize[p_ui.tex].y, TextureSize[p_ui.tex].x / p_ui.texturePos.w, TextureSize[p_ui.tex].y / p_ui.texturePos.h);
	entityData[entityCount].textureIndex = p_ui.tex;
	entityData[entityCount].layerIndex = p_ui.layer;
	entityData[entityCount].luminosity = glm::vec4(p_ui.luminosity.r, p_ui.luminosity.g, p_ui.luminosity.b, p_ui.luminosity.a);
	entityData[entityCount].color = glm::vec4(p_ui.color.r, p_ui.color.g, p_ui.color.b, p_ui.color.a);

	if (TexturesToRender.find(p_ui.layer) == TexturesToRender.end())
	{
		//TexturesToRender.push_back(p_ent.tex);
		std::vector<int> newVector;
		TexturesToRender.insert(std::pair<unsigned int, std::vector<int>>(p_ui.layer, newVector));
	}

	if (std::find(TexturesToRender[p_ui.layer].begin(), TexturesToRender[p_ui.layer].end(), p_ui.tex) == TexturesToRender[p_ui.layer].end())
	{
		TexturesToRender[p_ui.layer].push_back(p_ui.tex);
	}

	entityCount++;

	render(p_ui.uiText, false);
	render(p_ui.bar, false);
}

void OpenGLWindow::render(ui& p_ui) // i think this copys the texture to the renderer
{
	glm::mat4 transform = glm::mat4(1.0f);

	transform = glm::scale(transform, glm::vec3((p_ui.size.x) / getSize().x, (p_ui.size.y) / getSize().y, 0));  
	transform = glm::translate(transform, glm::vec3(((((p_ui.transform.x) + p_ui.size.x / 2) - getSize().x / 2)) / (p_ui.size.x / 2), -((((p_ui.transform.y) + p_ui.size.y / 2) - getSize().y / 2)) / (p_ui.size.y / 2), 0)); 

	entityData[entityCount].transform = transform;
	entityData[entityCount].textureCoordinates = glm::vec4(p_ui.texturePos.x / TextureSize[p_ui.tex].x, p_ui.texturePos.y / TextureSize[p_ui.tex].y, TextureSize[p_ui.tex].x / p_ui.texturePos.w, TextureSize[p_ui.tex].y / p_ui.texturePos.h);
	entityData[entityCount].textureIndex = p_ui.tex;
	entityData[entityCount].layerIndex = p_ui.layer;
	entityData[entityCount].luminosity = glm::vec4(p_ui.luminosity.r, p_ui.luminosity.g, p_ui.luminosity.b, p_ui.luminosity.a);
	entityData[entityCount].color = glm::vec4(p_ui.color.r, p_ui.color.g, p_ui.color.b, p_ui.color.a);

	if (TexturesToRender.find(p_ui.layer) == TexturesToRender.end())
	{
		//TexturesToRender.push_back(p_ent.tex);
		std::vector<int> newVector;
		TexturesToRender.insert(std::pair<unsigned int,std::vector<int>>(p_ui.layer, newVector));
	}

	if (std::find(TexturesToRender[p_ui.layer].begin(), TexturesToRender[p_ui.layer].end(), p_ui.tex) == TexturesToRender[p_ui.layer].end())
	{
		TexturesToRender[p_ui.layer].push_back(p_ui.tex);
	}

	entityCount++;
    
   	render(p_ui.uiText, false);
}

void OpenGLWindow::render(Light& p_light) // i think this copys the texture to the renderer
{
	Entity intersect(Vector2(p_light.radius, p_light.radius));
	intersect.transform = Vector2(p_light.transform.x - p_light.radius / 2, p_light.transform.y - p_light.radius / 2);

	if (intersect.intersecting(OnscreenCamera) == true && lightCount < maxLights)
	{
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::scale(transform, glm::vec3((p_light.radius) / getSize().x, (p_light.radius) / getSize().y, 0));  
		transform = glm::translate(transform, glm::vec3(((round(p_light.transform.x) - getSize().x / 2) - cameraOffset.x) / (p_light.radius / 2), -((round(p_light.transform.y) - getSize().y / 2) - cameraOffset.y) / (p_light.radius / 2), 0)); 
		transform = glm::rotate(transform, std::rad2deg(p_light.rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		lightData[lightCount].transform = transform;
		lightData[lightCount].color = glm::vec4(p_light.r, p_light.g, p_light.b, p_light.intensity);
		lightData[lightCount].layerIndex = p_light.layer;
		lightData[lightCount].shape = glm::vec2(p_light.shape.x, p_light.shape.y);

		transform = glm::mat4(1.0f);

		transform = glm::scale(transform, glm::vec3((p_light.radius) / getSize().x, (p_light.radius) / getSize().y, 0));  
		transform = glm::translate(transform, glm::vec3(((round(p_light.transform.x - p_light.radius / 2) - getSize().x / 2) - cameraOffset.x) / (p_light.radius / 2), -((round(p_light.transform.y  + p_light.radius / 2) - getSize().y / 2) - cameraOffset.y) / (p_light.radius / 2), 0)); 

		lightData[lightCount].emissionCenter = transform;

		lightCount++;
	}
}

void OpenGLWindow::display() // used to display information from the renderer to the window
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(VAO);

	//glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
 	
 	// position buffer
	glBindBuffer(GL_ARRAY_BUFFER, IVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(EntityGPUData), &entityData[0].transform, GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(EntityGPUData), (void*)0);
    glEnableVertexAttribArray(4);
   	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(EntityGPUData), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(EntityGPUData), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(EntityGPUData), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    // texture position buffer
    glBindBuffer(GL_ARRAY_BUFFER, IVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(EntityGPUData), &entityData[0].textureCoordinates, GL_STATIC_DRAW);

	glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(EntityGPUData), (void*)0);

    glVertexAttribDivisor(7, 1);

    // texture buffer
    glBindBuffer(GL_ARRAY_BUFFER, IVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(EntityGPUData), &entityData[0].textureIndex, GL_STATIC_DRAW);

	glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(EntityGPUData), (void*)0);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glVertexAttribDivisor(8, 1);

    // layer buffer
    glBindBuffer(GL_ARRAY_BUFFER, IVBO[3]);
    glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(EntityGPUData), &entityData[0].layerIndex, GL_STATIC_DRAW);

	glEnableVertexAttribArray(9);
    glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, sizeof(EntityGPUData), (void*)0);

    glVertexAttribDivisor(9, 1);

    // luminosity buffer
    glBindBuffer(GL_ARRAY_BUFFER, IVBO[4]);
	glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(EntityGPUData), &entityData[0].luminosity, GL_STATIC_DRAW);

	glEnableVertexAttribArray(10);
    glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(EntityGPUData), (void*)0);

    glVertexAttribDivisor(10, 1);

	// color buffer
	glBindBuffer(GL_ARRAY_BUFFER, IVBO[5]);
	glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(EntityGPUData), &entityData[0].color, GL_STATIC_DRAW);

	glEnableVertexAttribArray(11);
	glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(EntityGPUData), (void*)0);

	glVertexAttribDivisor(11, 1);

    // unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	defaultShader.use();

	defaultShader.setIntArray("ourTexture", 16, textureUnits);  

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, entityCount);

	glBindFramebuffer(GL_FRAMEBUFFER, FBOLight);
	luminosityShader.use();
	luminosityShader.setIntArray("ourTexture", 16, textureUnits); 
	glClear(GL_DEPTH_BUFFER_BIT);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, entityCount);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (int i = 0; i < lightCount; i++)
	{
		//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
		glStencilFunc( GL_ALWAYS, 1, 0xFF );

		shadowShader.use();

		shadowShader.setMat4("lightMatrix", lightData[i].emissionCenter);

		shadowShader.setInt("currentLayer", lightData[i].layerIndex);

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, entityCount);

		//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		glStencilFunc( GL_NOTEQUAL, 1, 0xFF );

		lightShader.use();

		lightShader.setInt("layerId", lightData[i].layerIndex);

		lightShader.setVec2("shape", lightData[i].shape);

		lightShader.setMat4("lightPos", lightData[i].transform);

		lightShader.setVec4("lightColor", lightData[i].color);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDepthMask(GL_TRUE);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);

	if (console.bloom == true)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOBlur); // back to default

		//first pass
		framebufferShader.use();
		framebufferShader.setFloat("zoom", 1.0f);
		framebufferShader.setInt("blurTexture", FBOTex);
		framebufferShader.setInt("lightTexture", FBOLightTex);
		framebufferShader.setFloat("pass", 1.0f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindTexture(GL_TEXTURE_2D, FBOBlurTex);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		//second pass
		framebufferShader.setFloat("pass", 2.0f);
		framebufferShader.setInt("blurTexture", FBOBlurTex);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default


		//third final pass
		framebufferShader.setFloat("zoom", zoom);
		framebufferShader.setInt("screenTexture", FBOTex);
		framebufferShader.setVec4("unlitColor", ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);
		framebufferShader.setFloat("pass", 3.0f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

		//4th secret final pass
		framebufferShader.use();
		framebufferShader.setFloat("zoom", zoom);
		framebufferShader.setInt("screenTexture", FBOTex);
		framebufferShader.setInt("lightTexture", FBOLightTex);
		framebufferShader.setVec4("unlitColor", ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);
		framebufferShader.setFloat("pass", 3.0f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	SDL_GL_SwapWindow(window);
}

void OpenGLWindow::quit() // used before exiting the program
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void OpenGLWindow::camera(Vector2 pos) // used before exiting the program
{
	cameraPos.lerp(cameraPos, pos, lerpAmount * Time::deltaTime());

	cameraPos.x = SDL_clamp(cameraPos.x, pos.x - clampAmount.x, pos.x + clampAmount.x);
	cameraPos.y = SDL_clamp(cameraPos.y, pos.y - clampAmount.y, pos.y + clampAmount.y);

	cameraOffset = Vector2(round(cameraPos.x) - ((getSize().x) / 2)  ,round(cameraPos.y) - ((getSize().y) / 2 ));
}

void OpenGLWindow::setZoom(float x)
{
	zoom = x;
	//SDL_RenderSetScale(renderer, x, x);
}

Vector2 OpenGLWindow::getSize()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	w = (w & ~1);
	h = (h & ~1);
	
	return Vector2(w, h);
	//return(Vector2(1024, 640));
}