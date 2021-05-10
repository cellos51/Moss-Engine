#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>

#include "renderwindow.hpp"

#include "player.hpp"

#include "event.hpp"

const short SCREEN_WIDTH = 1024;
const short SCREEN_HEIGHT = 640;

// random shit needed to be here to run
bool init();
bool gameRunning = true;

// main window
RenderWindow window;

// textures
SDL_Texture* playerTex;

std::vector<Entity> walls; // literally just walls

bool load = init(); // this is the end of textures and windows OK NVM

Player plr (Vector2(100, 0), playerTex, Vector2(64, 64));

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window.create("SDL Game", SCREEN_WIDTH, SCREEN_HEIGHT); // name and size of application window

	//textures
	playerTex = window.loadTexture("assets/textures/player.png");
	SDL_Texture* grass = window.loadTexture("assets/textures/grass.png");
	SDL_Texture* dirt1 = window.loadTexture("assets/textures/dirt1.png");
	SDL_Texture* dirt2 = window.loadTexture("assets/textures/dirt2.png");
	SDL_Texture* dirt3 = window.loadTexture("assets/textures/dirt3.png");
	SDL_Texture* dirt4 = window.loadTexture("assets/textures/dirt4.png");
	SDL_Texture* dirt5 = window.loadTexture("assets/textures/dirt5.png");
	SDL_Texture* dirt6 = window.loadTexture("assets/textures/dirt6.png");
	SDL_Texture* dirt7 = window.loadTexture("assets/textures/dirt7.png");
	SDL_Texture* dirt8 = window.loadTexture("assets/textures/dirt8.png"); // the texture used for the player
	SDL_Texture* dirt9 = window.loadTexture("assets/textures/dirt9.png");
	SDL_Texture* dirt10 = window.loadTexture("assets/textures/dirt10.png");
	SDL_Texture* dirt11 = window.loadTexture("assets/textures/dirt11.png");
	SDL_Texture* dirt12 = window.loadTexture("assets/textures/dirt12.png");
	SDL_Texture* dirt13 = window.loadTexture("assets/textures/dirt13.png");

	int level [10][16] = { // testing out a tile based level system

		{11,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9},
		{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
		{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
		{7,1,1,0,0,0,0,0,0,0,0,0,0,0,0,7},
		{7,3,12,1,1,1,0,0,0,1,1,1,0,0,0,7},
		{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
		{7,0,0,0,0,0,0,0,0,0,0,0,0,0,1,7},
		{7,0,0,0,0,0,0,0,1,0,0,0,0,1,5,6},
		{7,1,1,1,0,0,0,1,7,1,1,1,1,5,2,6},
		{10,3,3,12,1,1,1,10,3,3,3,3,3,3,3,12},
	};

	int a;
	int b;
	for(a = 0; a < 10; a++) // make these numbers the same as "level"
	{
		for(b = 0; b < 16; b++)
		{
			switch (level[a][b])
			{
				case 1:
				{
					Entity ent (Vector2(b * 64, a * 64), grass, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 2:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt1, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 3:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt2, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 4:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt3, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 5:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt4, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 6:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt5, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 7:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt6, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 8:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt7, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 9:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt8, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 10:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt9, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 11:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt10, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 12:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt11, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 13:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt12, Vector2(64,64));
					walls.push_back (ent);
				}
				break;

				case 14:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt13, Vector2(64,64));
					walls.push_back (ent);
				}
				break;
			}
		}
	}

	return true;
}

void gameLoop() // it runs forever
{
	float cameraX = plr.getPos().x - plr.getPos().x * 2 - window.camera.x + SCREEN_WIDTH / 2 - plr.getSize().x / 2;
	window.camera.x += cameraX / 10;
	float cameraY = plr.getPos().y - plr.getPos().y * 2 - window.camera.y + SCREEN_HEIGHT / 2 - plr.getSize().y / 2;
	window.camera.y += cameraY / 10;

	plr.goToMouse(Event::MousePressed(SDLK_LEFTMOUSE));

	if (Event::KeyPressed(SDLK_RIGHTARROW))
	{
		plr.velocity.x += 1;
	}

	if (Event::KeyPressed(SDLK_LEFTARROW))
	{
		plr.velocity.x += -1;
	}

	if (Event::KeyPressed(SDLK_UPARROW) && plr.touchground == true)
	{
		plr.touchground = false;
		plr.velocity.y = -25;
	}

	if (Event::KeyPressed(SDLK_DOWNARROW))
	{
		plr.velocity.y += 1;
	}
}

void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
	plr.update();
	
	for (Entity wall : walls)
	{
			plr.getCol(wall);
	}

	window.clear();
	window.render(plr);
	for (Entity wall : walls)
	{
		window.render(wall);
	}
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