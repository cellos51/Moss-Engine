#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "renderwindow.hpp"
#include "entity.hpp"
#include "player.hpp"

const short SCREEN_WIDTH = 1000;
const short SCREEN_HEIGHT = 600;

// random shit needed to be here to run
bool init();
bool gameRunning = true;

// main window
RenderWindow window;

// textures
SDL_Texture* playerTex;

bool load = init(); // this is the end of textures and windows OK NVM

Player player(Vector2(100, 100), playerTex);

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window.create("SDL Game", SCREEN_WIDTH, SCREEN_HEIGHT); // name and size of application window

	//textures
	playerTex = window.loadTexture("res/textures/player.png"); // the texture used for the player

	return true;
}

void gameLoop()
{
	SDL_Event event;
    while (SDL_PollEvent(&event)) // remember to put the game stuff in here
	{
		switch (event.type) 
		{
    		case SDL_QUIT: 
    		{
      			gameRunning = false;
      			break;
    		}
    		case SDL_MOUSEBUTTONDOWN:
    		{
    			if (event.button.button == SDL_BUTTON_LEFT)
   				{
   					player.goToMouse(true);
   				}
   				break;		
			}
			case SDL_MOUSEBUTTONUP:
    		{
    			if (event.button.button == SDL_BUTTON_LEFT)
   				{
   					player.goToMouse(false);
   				}
   				break;		
			}
		}
	}
	


	player.update();

	window.clear();

	window.render(player);

	window.display();
}

int main(int argc, char* args[])
{
	while (gameRunning) // main game loop ran every frame
	{ 
		gameLoop();
		SDL_Delay(16);
	}

	window.quit(); // run when use asks to exit program
	return 0;
}