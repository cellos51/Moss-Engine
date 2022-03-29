#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "renderwindow.hpp"
#include "entity.hpp"
#include "config.hpp"
#include "text.hpp"

float zoomX = 1, zoomY = 1;
Entity OnscreenCamera(Vector2(SCREEN_WIDTH,SCREEN_HEIGHT));

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

	OnscreenCamera.transform = Vector2(cameraPos.x - OnscreenCamera.size.x / 2, cameraPos.y - OnscreenCamera.size.y / 2);
	OnscreenCamera.size = Vector2(getSize().x / zoomX, getSize().y / zoomY);
}

void RenderWindow::render(Entity& p_ent, bool cam) // i think this copys the texture to the renderer
{
	if (p_ent.intersecting(OnscreenCamera) == true)
	{
		SDL_Rect src;
		src.x = p_ent.texturePos.x;
		src.y = p_ent.texturePos.y;
		src.w = p_ent.texturePos.w;
		src.h = p_ent.texturePos.h;	

		SDL_Rect dst;
		
		if (cam == true)
		{
			dst.x = (p_ent.offset.x + p_ent.transform.x) - cameraOffset.x;
			dst.y = (p_ent.offset.y + p_ent.transform.y) - cameraOffset.y;
			dst.w = p_ent.offset.w;
			dst.h = p_ent.offset.h;
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
}

void RenderWindow::render(Text& p_text, bool cam) // i think this copys the texture to the renderer
{
	if (p_text.getText().size() > 0 && p_text.font != NULL)
	{
		SDL_Rect dst;
		
		if (cam == true)
		{
			dst.x = (p_text.transform.x) - cameraOffset.x;
			dst.y = (p_text.transform.y) - cameraOffset.y;
			dst.w = (p_text.size.x);
			dst.h = (p_text.size.y);
		}
		else
		{
			dst.x = p_text.transform.x;
			dst.y = p_text.transform.y;
			dst.w = p_text.size.x;
			dst.h = p_text.size.y;
		}
		SDL_Surface* messageSurface = TTF_RenderText_Blended(p_text.font, p_text.getText().c_str(), p_text.color);
		SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, messageSurface);
		SDL_RenderCopy(renderer, Message, NULL, &dst);
		SDL_FreeSurface(messageSurface);
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
   	if (p_ui.uiText.getText().size() > 0 && p_ui.uiText.font != NULL)
	{
		SDL_Rect dst;
		
		dst.x = p_ui.uiText.transform.x + ((p_ui.transform.x) + ((p_ui.size.x / 2) - p_ui.uiText.size.x / 2));
		dst.y = p_ui.uiText.transform.y + ((p_ui.transform.y) + ((p_ui.size.y / 2) - p_ui.uiText.size.y / 2));
		dst.w = p_ui.uiText.size.x;
		dst.h = p_ui.uiText.size.y;
		
		SDL_Surface* messageSurface = TTF_RenderText_Blended(p_ui.uiText.font, p_ui.uiText.getText().c_str(), p_ui.uiText.color);
		SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, messageSurface);
		SDL_RenderCopy(renderer, Message, NULL, &dst);
		SDL_FreeSurface(messageSurface);
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

void RenderWindow::camera(Vector2 pos) // used before exiting the program
{
	SDL_RenderGetScale(renderer, &zoomX, &zoomY);

	float cameraX = pos.x + pos.x * -2 + cameraPos.x;
	cameraPos.x -= cameraX * 0.01 * Time::deltaTime();
	float cameraY = pos.y + pos.y * -2 + cameraPos.y;
	cameraPos.y -= cameraY * 0.01 * Time::deltaTime();

	cameraOffset = Vector2(cameraPos.x - ((getSize().x / zoomX) / 2)  , cameraPos.y - ((getSize().y / zoomY) / 2) );
}

void RenderWindow::setZoom(float x)
{
	SDL_RenderSetScale(renderer, x, x);
}

Vector2 RenderWindow::getSize()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	
	return Vector2(w, h);
}