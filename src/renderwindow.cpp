#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "renderwindow.hpp"
#include "entity.hpp"
#include "config.hpp"

void RenderWindow::create(const char* p_title, int p_w, int p_h)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
	src.x = p_ent.getCurrentFrame().x;
	src.y = p_ent.getCurrentFrame().y;
	src.w = p_ent.getCurrentFrame().w;
	src.h = p_ent.getCurrentFrame().h;	

	SDL_Rect dst;
	
	if (cam == true)
	{
		dst.x = p_ent.getPos().x + cameraPos.x;
		dst.y = p_ent.getPos().y + cameraPos.y;
		dst.w = p_ent.getCurrentFrame().w;
		dst.h = p_ent.getCurrentFrame().h;
	}
	else
	{
		dst.x = p_ent.getPos().x;
		dst.y = p_ent.getPos().y;
		dst.w = p_ent.getCurrentFrame().w;
		dst.h = p_ent.getCurrentFrame().h;
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
	float cameraX = p_ent.getPos().x - p_ent.getPos().x * 2 - cameraPos.x + SCREEN_WIDTH / 2 - p_ent.getSize().x / 2;
	cameraPos.x += cameraX / 10;
	float cameraY = p_ent.getPos().y - p_ent.getPos().y * 2 - cameraPos.y + SCREEN_HEIGHT / 2 - p_ent.getSize().y / 2;
	cameraPos.y += cameraY / 10;
}