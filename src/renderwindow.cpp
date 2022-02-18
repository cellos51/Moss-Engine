#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "renderwindow.hpp"
#include "entity.hpp"
#include "config.hpp"
#include "text.hpp"

void RenderWindow::create(const char* p_title, int p_w, int p_h)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) // used load textures :P
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);
	return texture;
}

void RenderWindow::clear() // clears the renderer
{
	SDL_SetRenderDrawColor(renderer, 44, 47, 51, 0);
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_ent, bool cam) // i think this copys the texture to the renderer
{
	SDL_Rect src;
	src.x = p_ent.texturePos.x;
	src.y = p_ent.texturePos.y;
	src.w = p_ent.texturePos.w;
	src.h = p_ent.texturePos.h;	

	SDL_Rect dst;
	
	if (cam == true)
	{
		dst.x = ((p_ent.offset.x + p_ent.transform.x) - zoom * p_ent.transform.x) + cameraPos.x - zoom * cameraPos.x;
		dst.y = ((p_ent.offset.y + p_ent.transform.y) - zoom * p_ent.transform.y) + cameraPos.y - zoom * cameraPos.y;
		dst.w = p_ent.offset.w - p_ent.offset.w * zoom;
		dst.h = p_ent.offset.h  - p_ent.offset.w * zoom;
	}
	else
	{
		dst.x = p_ent.transform.x + p_ent.offset.x;
		dst.y = p_ent.transform.y + p_ent.offset.x;
		dst.w = p_ent.offset.w;
		dst.h = p_ent.offset.h;
	}


	if (p_ent.getTex() != NULL)
	{
		SDL_RenderCopy(renderer, p_ent.getTex(), &src, &dst);
	}
	else
	{
		SDL_SetRenderDrawColor( renderer, 153, 170, 181, 0);        
    	SDL_RenderFillRect( renderer, &dst );
	}
}

void RenderWindow::render(Text& p_text, bool cam) // i think this copys the texture to the renderer
{
	if (p_text.getText().size() > 0)
	{
		SDL_Rect dst;
		
		if (cam == true)
		{
			dst.x = (p_text.transform.x - zoom * p_text.transform.x) + cameraPos.x - zoom * cameraPos.x;
			dst.y = (p_text.transform.y - zoom * p_text.transform.y) + cameraPos.y - zoom * cameraPos.y;
			dst.w = (p_text.size.x) - (p_text.size.x) * zoom;
			dst.h = (p_text.size.y) - (p_text.size.y) * zoom;
		}
		else
		{
			dst.x = p_text.transform.x;
			dst.y = p_text.transform.y;
			dst.w = p_text.size.x;
			dst.h = p_text.size.y;
		}
		SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, p_text.messageSurface);
		SDL_RenderCopy(renderer, Message, NULL, &dst);
		SDL_DestroyTexture(Message);
	}	
}

void RenderWindow::render(ui& p_ui) // i think this copys the texture to the renderer
{
	SDL_Rect dst;
	
	dst.x = p_ui.transform.x;
	dst.y = p_ui.transform.y;
	dst.w = p_ui.size.x;
	dst.h = p_ui.size.y;

	SDL_SetRenderDrawColor( renderer, p_ui.red, p_ui.green, p_ui.blue, 0);
    SDL_RenderFillRect( renderer, &dst );

   	// render(p_ui.uiText, false);
   	if (p_ui.uiText.getText().size() > 0)
	{
		SDL_Rect dst;
		
		dst.x = p_ui.uiText.transform.x + ((p_ui.transform.x) + ((p_ui.size.x / 2) - p_ui.uiText.size.x / 2));
		dst.y = p_ui.uiText.transform.y + ((p_ui.transform.y) + ((p_ui.size.y / 2) - p_ui.uiText.size.y / 2));
		dst.w = p_ui.uiText.size.x;
		dst.h = p_ui.uiText.size.y;

		SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, p_ui.uiText.messageSurface);
		SDL_RenderCopy(renderer, Message, NULL, &dst);
		SDL_DestroyTexture(Message);
	}	
}

void RenderWindow::display() // used to display information from the renderer to the window
{
	SDL_RenderPresent(renderer);
}

void RenderWindow::quit() // used before exiting the program
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void RenderWindow::camera(Entity& p_ent) // used before exiting the program
{
	float cameraX = p_ent.transform.x - p_ent.transform.x * 2 + zoom - cameraPos.x + getSize().x / (2 - zoom * 2) - p_ent.size.x / (2 - zoom * 2);
	cameraPos.x += cameraX * 0.01 * Time::deltaTime();
	float cameraY = p_ent.transform.y - p_ent.transform.y * 2 + zoom - cameraPos.y + getSize().y / (2 - zoom * 2) - p_ent.size.y / (2 - zoom * 2);
	cameraPos.y += cameraY * 0.01 * Time::deltaTime();
}

Vector2 RenderWindow::getSize()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	return Vector2(w, h);
}