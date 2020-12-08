#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "renderwindow.hpp"
#include "entity.hpp"

void RenderWindow::create(const char* p_title, int p_w, int p_h)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(renderer, 44, 47, 51, 0);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) // used load textures :P
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);
	return texture;
}

void RenderWindow::clear() // clears the renderer
{
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_ent) // i think this copys the texture to the renderer
{
	SDL_Rect src;
	src.x = p_ent.getCurrentFrame().x;
	src.y = p_ent.getCurrentFrame().y;
	src.w = p_ent.getCurrentFrame().w;
	src.h = p_ent.getCurrentFrame().h;	

	SDL_Rect dst;
	dst.x = p_ent.transform.x;
	dst.y = p_ent.transform.y;
	dst.w = p_ent.getCurrentFrame().w * 4;
	dst.h = p_ent.getCurrentFrame().h * 4;

	SDL_RenderCopy(renderer, p_ent.getTex(), &src, &dst);
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