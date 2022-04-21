#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // removing later
#include <stb_image.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "renderwindow.hpp"
#include "shader.hpp"
#include "math.hpp"
#include "entity.hpp"
#include "config.hpp"
#include "text.hpp"

float zoomX = 1, zoomY = 1;
Entity OnscreenCamera(Vector2(SCREEN_WIDTH,SCREEN_HEIGHT));

Shader ourShader("assets/shaders/shader.vs", "assets/shaders/shader.fs");

int width, height, nrChannels;
unsigned char *data;
unsigned int texture;


void RenderWindow::create(const char* p_title, int p_w, int p_h)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	context = SDL_GL_CreateContext(window);
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	glViewport(0, 0, getSize().x, getSize().y);
	//window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	//renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	float vertices[] = {
    // positions          // colors           // texture coords
     1,  1, 0.0f,   1, 1, 1,   1.0f, 1.0f,   // top right
     1, -1, 0.0f,   1, 1, 1,   1.0f, 0.0f,   // bottom right
    -1, -1, 0.0f,   1, 1, 1,   0.0f, 0.0f,   // bottom left
    -1,  1, 0.0f,   1, 1, 1,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
	};  


	stbi_set_flip_vertically_on_load(true); 
	data = stbi_load("assets/textures/player.png", &width, &height, &nrChannels, 0); 


	glGenTextures(1, &texture);  
	glBindTexture(GL_TEXTURE_2D, texture); 

	if (data)
	{
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	    //glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
	    std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ourShader.compile();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
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

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

unsigned int RenderWindow::loadTexture(const char* p_filePath) // used load textures :P
{
	stbi_set_flip_vertically_on_load(true); 

	unsigned char* data1 = stbi_load("assets/textures/player.png", &width, &height, &nrChannels, 0); 
	unsigned int texture2;


	glGenTextures(1, &texture);  
	glBindTexture(GL_TEXTURE_2D, texture); 

	if (data)
	{
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	    //glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
	    return NULL;
	}
	stbi_image_free(data);

	return texture;
}

void RenderWindow::clear() // clears the renderer
{
	//SDL_SetRenderDrawColor(renderer, 44, 47, 51, 0);
	//SDL_RenderClear(renderer);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	OnscreenCamera.transform = Vector2(cameraPos.x - OnscreenCamera.size.x / 2, cameraPos.y - OnscreenCamera.size.y / 2);
	OnscreenCamera.size = Vector2(getSize().x / zoomX, getSize().y / zoomY);
}

void RenderWindow::render(Entity& p_ent, bool cam) // i think this copys the texture to the renderer
{
	if (p_ent.intersecting(OnscreenCamera) == true)
	{
		ourShader.use();  

		glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(p_ent.size.x / getSize().x, p_ent.size.y / getSize().y, 0));  

		//trans = glm::translate(trans, glm::vec3((((p_ent.offset.x + p_ent.transform.x) - p_ent.size.x / 2) - getSize().x / 2) / 8, -(((p_ent.offset.y + p_ent.transform.y) - p_ent.size.y / 2) - getSize().y / 2) / 8, 0)); 

		trans = glm::translate(trans, glm::vec3(((((p_ent.offset.x + p_ent.transform.x) + p_ent.size.x / 2) - getSize().x / 2) - cameraOffset.x) / (p_ent.size.x / 2), -((((p_ent.offset.y + p_ent.transform.y) + p_ent.size.y / 2) - getSize().y / 2) - cameraOffset.y) / (p_ent.size.y / 2), 0)); 

		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
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
	// SDL_Rect dst;
	
	// dst.x = p_ui.transform.x;
	// dst.y = p_ui.transform.y;
	// dst.w = p_ui.size.x;
	// dst.h = p_ui.size.y;
	
	// SDL_SetRenderDrawColor( renderer, p_ui.red, p_ui.green, p_ui.blue, 0);
 //    SDL_RenderFillRect( renderer, &dst );
    
 //   	// render(p_ui.uiText, false);
 //   	if (p_ui.uiText.getText().size() > 0 && p_ui.uiText.font != NULL)
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




void RenderWindow::display() // used to display information from the renderer to the window
{
	

	glViewport(0, 0, getSize().x, getSize().y);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	SDL_GL_SwapWindow(window);
	//SDL_RenderPresent(renderer);
}

void RenderWindow::quit() // used before exiting the program
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void RenderWindow::camera(Vector2 pos) // used before exiting the program
{
	// SDL_RenderGetScale(renderer, &zoomX, &zoomY);

	float cameraX = pos.x + pos.x * -2 + cameraPos.x;
	cameraPos.x -= cameraX * 0.01 * Time::deltaTime();
	float cameraY = pos.y + pos.y * -2 + cameraPos.y;
	cameraPos.y -= cameraY * 0.01 * Time::deltaTime();

	cameraOffset = Vector2(cameraPos.x - ((getSize().x / zoomX) / 2)  , cameraPos.y - ((getSize().y / zoomY) / 2) );
}

void RenderWindow::setZoom(float x)
{
	//SDL_RenderSetScale(renderer, x, x);
}

Vector2 RenderWindow::getSize()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	
	return Vector2(w, h);
	return(Vector2(1024, 640));
}