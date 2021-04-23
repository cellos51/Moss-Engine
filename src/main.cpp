#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>

#include "renderwindow.hpp"

#include "player.hpp"

#include "event.hpp"

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

Entity wall (Vector2(500, 300), Vector2(128, 128));

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
	for (int i = players.size(); i > 0; i--) // picks up players
	{
		players[i - 1].goToMouse(Event::MousePressed(SDLK_LEFTMOUSE));
	}

	if (Event::MousePressed(SDLK_RIGHTMOUSE)) // spawns players
	{
		int mX, mY;
    	SDL_GetMouseState(&mX, &mY);
		Player plr (Vector2(mX - 32, mY - 32), playerTex, Vector2(64, 64));
		players.push_back(plr);
	}

	if (Event::MousePressed(SDLK_MIDDLEMOUSE)) // deletes players
	{
		int i = 0;
   		for (Player& plr : players)
   			{
   			if (plr.touchingMouse())
   				{
   					players.erase(players.begin() + i);
   				}
   				i++;
   			}
   		players.shrink_to_fit();
	}

	for (Player& plr : players) // make a wild guess dumbass
	{
		plr.update();
	}

	for (Player& plr : players) // physics calculations for collisions
  	{
  		plr.getCol(wall);
  	}
}

void render()
{
  window.clear();
  for (Player& plr : players)
  {
    window.render(plr);
  }
  window.render(wall);
  window.display();
}

int main(int argc, char* args[])
{
	while (gameRunning) // main game loop ran every frame
	{
    	Event::PollEvent();
    	gameRunning = Event::AppQuit();
		gameLoop();
    	render();
		SDL_Delay(16);
	}

	window.quit(); // run when user asks to exit program
	return 0;
}