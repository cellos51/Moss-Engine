#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>

#include "math.hpp"

#include "renderwindow.hpp"

#include "player.hpp"

#include "event.hpp"

#include "level.hpp"

#include "config.hpp" // this just has the screen size so if i change it i dont need to change it in every class

// random shit needed to be here to run
bool init();
bool gameRunning = true;

// main window
RenderWindow window;

// camra offset used for panning
Vector2 offsetCam;
Vector2 offsetMouse;

// textures
SDL_Texture* playerTex;
SDL_Texture* tileSet[14];

std::vector<Entity> walls; // literally just walls (for the level) (also why the fuck don't i make a seperete entity derived class for the level??? ahh fuck it)

Vector2 PlayerSpawn = Vector2(0,0);

bool load = init(); // this is the end of textures and windows OK NVM


Player plr (PlayerSpawn, window.loadTexture("assets/textures/player.png"), Vector2(64,64));

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window.create("Moss Engine", SCREEN_WIDTH, SCREEN_HEIGHT); // name and size of application window

	// textures
	tileSet[0] = window.loadTexture("assets/textures/grass.png");
	tileSet[1] = window.loadTexture("assets/textures/dirt1.png");
	tileSet[2] = window.loadTexture("assets/textures/dirt2.png");
	tileSet[3] = window.loadTexture("assets/textures/dirt3.png");
	tileSet[4] = window.loadTexture("assets/textures/dirt4.png");
	tileSet[5] = window.loadTexture("assets/textures/dirt5.png");
	tileSet[6] = window.loadTexture("assets/textures/dirt6.png");
	tileSet[7] = window.loadTexture("assets/textures/dirt7.png");
	tileSet[8] = window.loadTexture("assets/textures/dirt8.png"); 
	tileSet[9] = window.loadTexture("assets/textures/dirt9.png");
	tileSet[10] = window.loadTexture("assets/textures/dirt10.png");
	tileSet[11] = window.loadTexture("assets/textures/dirt11.png");
	tileSet[12] = window.loadTexture("assets/textures/dirt12.png");
	tileSet[13] = window.loadTexture("assets/textures/dirt13.png");

	PlayerSpawn = Level::LoadLevel(Level::LoadFile("assets/levels/level1.lvl"), walls, window, tileSet);

	return true;
}

void gameLoop() // it runs forever
{
	window.camera(plr);

	if (Event::KeyPressed(SDLK_1))
	{
		window.zoom = 0;
	}
	else if (Event::KeyPressed(SDLK_2))
	{
		window.zoom = 0.25;
	}
	else if (Event::KeyPressed(SDLK_3))
	{
		window.zoom = 0.5;
	}
	else if (Event::KeyPressed(SDLK_4))
	{
		window.zoom = 0.75;
	}	

	plr.update();
	
	for (Entity wall : walls)
	{
		plr.getCol(wall);
	}
}


void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
	window.clear();
	window.render(plr, true);

	for (Entity wall : walls)
	{
		window.render(wall, true);
	}
	window.display();
}

int main(int argc, char* args[])
{
	while (gameRunning) // main game loop ran every frame
	{
		Time::Tick();
    	Event::PollEvent();
    	gameRunning = Event::AppQuit();
		gameLoop();
    	render();
	}

	window.quit(); // run when user asks to exit program
	return 0;
}