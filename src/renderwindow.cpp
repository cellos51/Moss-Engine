#include "renderwindow.hpp"

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

float zoom = 1;
Entity OnscreenCamera(Vector2(0,0));

Shader framebufferShader("assets/shaders/framebuffer.vs", "assets/shaders/framebuffer.fs");

Shader defaultShader("assets/shaders/shader.vs", "assets/shaders/shader.fs");

Shader shadowShader("assets/shaders/shadowshader.vs", "assets/shaders/shadowshader.fs");
Shader lightShader("assets/shaders/lightshader.vs", "assets/shaders/lightshader.fs");

unsigned int newestTexture = 0;

std::map<unsigned int,Vector2> TextureSize;

const int maxEntities = 10000;

const int maxLights = 1000;

int entityCount = 0; // begin stuff for entities

glm::mat4 positionArray[maxEntities];

glm::vec4 texCoordArray[maxEntities];

int textureArray[maxEntities];

unsigned int layerArray[maxEntities];

bool shadowArray[maxEntities];

int lightCount = 0; // begin stuff for lights

glm::vec4 lightColorArray[maxLights];

glm::mat4 lightPositionArray[maxLights];

glm::mat4 shadowPositionArray[maxLights];

unsigned int lightLayerArray[maxLights];

std::map<unsigned int,std::vector<int>> TexturesToRender; // this does stuff

void RenderWindow::create(const char* p_title, int p_w, int p_h)
{
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

	int glConsts;
	
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &glConsts);
	std::cout << "Max Uniform Block Size:" << glConsts << std::endl;
	
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &glConsts);
	std::cout << "Max Vertex Unifrom Components:" << glConsts << std::endl;

	glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &glConsts);
	std::cout << "Max Vertex Output Components:" << glConsts << std::endl;
	

	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &glConsts);
	std::cout << "Max Fragment Unifrom Components:" << glConsts << std::endl;

	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &glConsts);
	std::cout << "Max Unifrom Components:" << glConsts << std::endl;

	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &glConsts);
	std::cout << "Max Array Texture Layers:" << glConsts << std::endl;

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
	glGenTextures(1, &FBOTex);
	glGenTextures(1, &FBOLightTex);
	glGenRenderbuffers(1, &RBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &IVBO[0]); // position
    glGenBuffers(1, &IVBO[1]); // texture position
    glGenBuffers(1, &IVBO[2]); // texture 
    glGenBuffers(1, &IVBO[3]); // layer
    glGenBuffers(1, &IVBO[4]); // shadow

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glActiveTexture(GL_TEXTURE0 + FBOTex);
    glBindTexture(GL_TEXTURE_2D, FBOTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_w, p_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTex, 0); 
	//glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);  
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, p_w, p_h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);  

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, FBOLight);

    glActiveTexture(GL_TEXTURE0 + FBOLightTex);
    glBindTexture(GL_TEXTURE_2D, FBOLightTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p_w, p_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOLightTex, 0); 
	//glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);  
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, p_w, p_h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);  

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

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
}

unsigned int test = 0;

unsigned int RenderWindow::loadTexture(const char* p_filePath) // used load textures :P
{
	int width, height, nrChannels;
	unsigned int texture;
	unsigned char *data;

	stbi_set_flip_vertically_on_load(true); 

	data = stbi_load(p_filePath, &width, &height, &nrChannels, 0); 


	glGenTextures(1, &texture);  
	glActiveTexture(GL_TEXTURE0 + texture);
	glBindTexture(GL_TEXTURE_2D, texture);

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	TextureSize.insert(std::pair<unsigned int,Vector2>(texture, Vector2(width, height)));

	std::cout << texture << std::endl;

	newestTexture = texture;
	return texture;
}

void RenderWindow::clear() // clears the renderer
{
	entityCount = 0;
	lightCount = 0;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.439, 0.502, 0.565, 1.0);
	//glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, FBOLight);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	//glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	OnscreenCamera.transform = Vector2(cameraPos.x - OnscreenCamera.size.x / 2, cameraPos.y - OnscreenCamera.size.y / 2);
	OnscreenCamera.size = Vector2(getSize().x / zoom, getSize().y / zoom);
}

void RenderWindow::render(Entity& p_ent, bool cam) // i think this copys the texture to the renderer
{
	if (p_ent.intersecting(OnscreenCamera) == true && cam == true && entityCount < maxEntities)
	{
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::scale(transform, glm::vec3((p_ent.offset.w) / getSize().x, (p_ent.offset.h) / getSize().y, 0));  
		transform = glm::translate(transform, glm::vec3(((((p_ent.offset.x + round(p_ent.transform.x)) + p_ent.offset.w / 2) - getSize().x / 2) - cameraOffset.x) / (p_ent.offset.w / 2), -((((p_ent.offset.y + round(p_ent.transform.y)) + p_ent.offset.h / 2) - getSize().y / 2) - cameraOffset.y) / (p_ent.offset.h / 2), 0)); 

		positionArray[entityCount] = transform;
		texCoordArray[entityCount] = glm::vec4(p_ent.texturePos.x / TextureSize[p_ent.tex].x,p_ent.texturePos.y / TextureSize[p_ent.tex].y, TextureSize[p_ent.tex].x / p_ent.texturePos.w, TextureSize[p_ent.tex].y / p_ent.texturePos.h);
		textureArray[entityCount] = p_ent.tex;
		layerArray[entityCount] = p_ent.layer;
		shadowArray[entityCount] = p_ent.shadow;

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


		positionArray[entityCount] = transform;
		texCoordArray[entityCount] = glm::vec4(p_ent.texturePos.x / TextureSize[p_ent.tex].x,p_ent.texturePos.y / TextureSize[p_ent.tex].y, TextureSize[p_ent.tex].x / p_ent.texturePos.w, TextureSize[p_ent.tex].y / p_ent.texturePos.h);
		textureArray[entityCount] = p_ent.tex;
		layerArray[entityCount] = p_ent.layer;
		shadowArray[entityCount] = p_ent.shadow;

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

	// if (entityCount >= maxEntities)
	// {
	// 	defaultShader.use();  
	
	// 	glUniformMatrix4fv(glGetUniformLocation(defaultShader.ID, "transform"), entityCount, GL_FALSE, glm::value_ptr(positionArray[0]));

	// 	//glUniform4fv(glGetUniformLocation(defaultShader.ID, "texOffset"), entityCount, glm::value_ptr(texCoordArray[0]));

	// 	//glUniform1iv(glGetUniformLocation(defaultShader.ID, "textureId"), entityCount, textureArray);

	// 	int test[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	// 	glUniform1iv(glGetUniformLocation(defaultShader.ID, "ourTexture"), 16, test);

	// 	glBindVertexArray(VAO);      

	// 	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, entityCount);

	// 	entityCount = 0;
	// }

	// if (p_ent.intersecting(OnscreenCamera) == true)
	// {
	// 	SDL_Rect src;
	// 	src.x = p_ent.texturePos.x;
	// 	src.y = p_ent.texturePos.y;
	// 	src.w = p_ent.texturePos.w;
	// 	src.h = p_ent.texturePos.h;	

	// 	SDL_Rect dst;
		
	// 	if (cam == true)
	// 	{
	// 		dst.x = (p_ent.offset.x + p_ent.transform.x) - cameraOffset.x;
	// 		dst.y = (p_ent.offset.y + p_ent.transform.y) - cameraOffset.y;
	// 		dst.w = p_ent.offset.w;
	// 		dst.h = p_ent.offset.h;
	// 	}
	// 	else
	// 	{
	// 		dst.x = p_ent.transform.x + p_ent.offset.x;
	// 		dst.y = p_ent.transform.y + p_ent.offset.x;
	// 		dst.w = p_ent.offset.w;
	// 		dst.h = p_ent.offset.h;
	// 	}


	// 	if (p_ent.getTex() != NULL)
	// 	{
	// 		SDL_RenderCopy(renderer, p_ent.getTex(), &src, &dst);
	// 	}
	// 	else
	// 	{
	// 		SDL_SetRenderDrawColor( renderer, 153, 170, 181, 0);        
	//     	SDL_RenderFillRect( renderer, &dst );
	// 	}
	// }
}

void RenderWindow::render(Text& p_text, bool cam) // i think this copys the texture to the renderer
{
	// if (p_text.getText().size() > 0 && p_text.font != NULL)
	// {
	// 	SDL_Rect dst;
		
	// 	if (cam == true)
	// 	{
	// 		dst.x = (p_text.transform.x) - cameraOffset.x;
	// 		dst.y = (p_text.transform.y) - cameraOffset.y;
	// 		dst.w = (p_text.size.x);
	// 		dst.h = (p_text.size.y);
	// 	}
	// 	else
	// 	{
	// 		dst.x = p_text.transform.x;
	// 		dst.y = p_text.transform.y;
	// 		dst.w = p_text.size.x;
	// 		dst.h = p_text.size.y;
	// 	}
	// 	SDL_Surface* messageSurface = TTF_RenderText_Blended(p_text.font, p_text.getText().c_str(), p_text.color);
	// 	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, messageSurface);
	// 	SDL_RenderCopy(renderer, Message, NULL, &dst);
	// 	SDL_FreeSurface(messageSurface);
	// 	SDL_DestroyTexture(Message);
	// }	
}

void RenderWindow::render(ui& p_ui) // i think this copys the texture to the renderer
{
	glm::mat4 transform = glm::mat4(1.0f);

	transform = glm::scale(transform, glm::vec3((p_ui.size.x) / getSize().x, (p_ui.size.y) / getSize().y, 0));  
	transform = glm::translate(transform, glm::vec3(((((p_ui.transform.x) + p_ui.size.x / 2) - getSize().x / 2)) / (p_ui.size.x / 2), -((((p_ui.transform.y) + p_ui.size.y / 2) - getSize().y / 2)) / (p_ui.size.y / 2), 0)); 

	positionArray[entityCount] = transform;
	texCoordArray[entityCount] = glm::vec4(1,1,1,1);
	textureArray[entityCount] = p_ui.layer;
	layerArray[entityCount] = p_ui.tex;
	shadowArray[entityCount] = false;

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

	// SDL_Rect dst;
	
	// dst.x = p_ui.transform.x;
	// dst.y = p_ui.transform.y;
	// dst.w = p_ui.size.x;
	// dst.h = p_ui.size.y;
	
	//SDL_SetRenderDrawColor( renderer, p_ui.red, p_ui.green, p_ui.blue, 0);
    //SDL_RenderFillRect( renderer, &dst );
    
   	// render(p_ui.uiText, false);
 	// if (p_ui.uiText.getText().size() > 0 && p_ui.uiText.font != NULL)
	// {
	// 	SDL_Rect dst;
		
	// 	dst.x = p_ui.uiText.transform.x + ((p_ui.transform.x) + ((p_ui.size.x / 2) - p_ui.uiText.size.x / 2));
	// 	dst.y = p_ui.uiText.transform.y + ((p_ui.transform.y) + ((p_ui.size.y / 2) - p_ui.uiText.size.y / 2));
	// 	dst.w = p_ui.uiText.size.x;
	// 	dst.h = p_ui.uiText.size.y;
		
	// 	SDL_Surface* messageSurface = TTF_RenderText_Blended(p_ui.uiText.font, p_ui.uiText.getText().c_str(), p_ui.uiText.color);
	// 	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, messageSurface);
	// 	SDL_RenderCopy(renderer, Message, NULL, &dst);
	// 	SDL_FreeSurface(messageSurface);
	// 	SDL_DestroyTexture(Message);
	// }
}

void RenderWindow::render(Light& p_light) // i think this copys the texture to the renderer
{
	if (lightCount < maxLights)
	{
		glm::mat4 transform = glm::mat4(1.0f);

		transform = glm::scale(transform, glm::vec3((p_light.radius) / getSize().x, (p_light.radius) / getSize().y, 0));  
		transform = glm::translate(transform, glm::vec3(((round(p_light.transform.x) - getSize().x / 2) - cameraOffset.x) / (p_light.radius / 2), -((round(p_light.transform.y) - getSize().y / 2) - cameraOffset.y) / (p_light.radius / 2), 0)); 

		lightColorArray[lightCount] = glm::vec4(p_light.r, p_light.g, p_light.b, p_light.intensity);
		lightPositionArray[lightCount] = transform;
		lightLayerArray[lightCount] = p_light.layer;

		transform = glm::mat4(1.0f);

		transform = glm::scale(transform, glm::vec3((p_light.radius) / getSize().x, (p_light.radius) / getSize().y, 0));  
		transform = glm::translate(transform, glm::vec3(((round(p_light.transform.x - p_light.radius / 2) - getSize().x / 2) - cameraOffset.x) / (p_light.radius / 2), -((round(p_light.transform.y  + p_light.radius / 2) - getSize().y / 2) - cameraOffset.y) / (p_light.radius / 2), 0)); 


		shadowPositionArray[lightCount] = transform;

		lightCount++;
	}
}

void RenderWindow::display() // used to display information from the renderer to the window
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(VAO);

	glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

	glViewport(0, 0, getSize().x, getSize().y);

	glBindTexture(GL_TEXTURE_2D, FBOTex);	  
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getSize().x, getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, FBOLightTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getSize().x, getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, RBO);  
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, getSize().x, getSize().y);

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
 	
 	// position buffer
	glBindBuffer(GL_ARRAY_BUFFER, IVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(glm::mat4), &positionArray[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(4);
   	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    // texture position buffer
    glBindBuffer(GL_ARRAY_BUFFER, IVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(glm::vec4), &texCoordArray[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

    glVertexAttribDivisor(7, 1);

    // texure buffer
    glBindBuffer(GL_ARRAY_BUFFER, IVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(int), &textureArray[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(int), (void*)0);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glVertexAttribDivisor(8, 1);

    // layer buffer
    glBindBuffer(GL_ARRAY_BUFFER, IVBO[3]);
    glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(int), &layerArray[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(9);
    glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, sizeof(int), (void*)0);

    glVertexAttribDivisor(9, 1);

    // shadow buffer
    glBindBuffer(GL_ARRAY_BUFFER, IVBO[4]);
	glBufferData(GL_ARRAY_BUFFER, entityCount * sizeof(int), &shadowArray[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(10);
    glVertexAttribIPointer(10, 1, GL_UNSIGNED_BYTE, GL_FALSE, (void*)0);
    //glVertexAttribIPointer(3, 1, GL_UNSIGNED_BYTE, GL_FALSE, (void*)(8 * sizeof(bool)));

    glVertexAttribDivisor(10, 1);


    // unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, newestTexture);

	defaultShader.use(); 

	int bruh[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

	defaultShader.setIntArray("ourTexture", 16, bruh);  

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, entityCount);


	glBindFramebuffer(GL_FRAMEBUFFER, FBOLight);
	for (int i = 0; i < lightCount; i++)
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
		glStencilFunc( GL_ALWAYS, 1, 0xFF );

		shadowShader.use();

		shadowShader.setMat4("lightMatrix", shadowPositionArray[i]); 

		shadowShader.setInt("currentLayer", lightLayerArray[i]);

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, entityCount);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		glStencilFunc( GL_NOTEQUAL, 1, 0xFF );

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		lightShader.use();

		lightShader.setInt("layerId", lightLayerArray[i]);

		lightShader.setMat4("lightPos", lightPositionArray[i]);

		lightShader.setVec4("lightColor", lightColorArray[i]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDepthMask(GL_TRUE);

		glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
		glStencilFunc( GL_ALWAYS, 1, 0xFF );

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default

	glDisable(GL_DEPTH_TEST);

	//glBindTexture(GL_TEXTURE_2D, FBOTex);

	framebufferShader.use();
	framebufferShader.setFloat("zoom", zoom);
	framebufferShader.setInt("screenTexture", FBOTex);
	framebufferShader.setInt("lightTexture", FBOLightTex);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	SDL_GL_SwapWindow(window);
}

void RenderWindow::quit() // used before exiting the program
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int clampAmount = 5;
float lerpAmount = 0.005;

void RenderWindow::camera(Vector2 pos) // used before exiting the program
{
	cameraPos.lerp(cameraPos, pos, lerpAmount * Time::deltaTime());

	cameraPos.x = std::clamp(cameraPos.x, pos.x - clampAmount, pos.x + clampAmount);
	cameraPos.y = std::clamp(cameraPos.y, pos.y - clampAmount, pos.y + clampAmount);

	cameraOffset = Vector2(round(cameraPos.x) - ((getSize().x) / 2)  ,round(cameraPos.y) - ((getSize().y) / 2 ));
}

void RenderWindow::setZoom(float x)
{
	zoom = x;
	//SDL_RenderSetScale(renderer, x, x);
}

Vector2 RenderWindow::getSize()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	w = (w & ~1);
	h = (h & ~1);
	
	return Vector2(w, h);
	//return(Vector2(1024, 640));
}