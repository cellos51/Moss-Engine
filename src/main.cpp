#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

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

std::vector<Player> players;

bool createPlayers;

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
   					for (Player& plr : players)
   					{
   						plr.goToMouse(true);
   					}
   				}	
   				if (event.button.button == SDL_BUTTON_RIGHT)
   				{
   					createPlayers = true;
   				}
   				break;
			}
			case SDL_MOUSEBUTTONUP:
    		{
    			if (event.button.button == SDL_BUTTON_LEFT)
   				{
   					for (Player& plr : players)
   					{
   						plr.goToMouse(false);
   					}
   					
   				}
   				if (event.button.button == SDL_BUTTON_RIGHT)
   				{
   					createPlayers = false;
   				}
   				break;		
			}
		}
	}
	
	if (createPlayers == true)
	{
		int mX, mY;
	   	SDL_GetMouseState(&mX, &mY);
		Player plr (Vector2(mX - plr.getSize() / 2, mY - plr.getSize() / 2), playerTex);
		players.push_back(plr);
	}

	window.clear();

	for (Player& plr : players)
	{
		plr.update();
		window.render(plr);
	}

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