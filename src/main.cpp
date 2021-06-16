#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

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
SDL_Texture* tileSet[13];

std::vector<Entity> walls; // literally just walls (for the level) (also why the fuck don't i make a seperete entity derived class for the level??? ahh fuck it)

bool load = init(); // this is the end of textures and windows OK NVM

Player plr (Vector2(100, 0), playerTex, Vector2(64, 64));

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window.create("SDL Game", SCREEN_WIDTH, SCREEN_HEIGHT); // name and size of application window

	// textures
	playerTex = window.loadTexture("assets/textures/player.png"); // the texture used for the player
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



	Level::LoadLevel(Level::LoadFile("assets/levels/level1.lvl"), walls, window, tileSet);

	return true;
}

void gameLoop() // it runs forever
{
	window.camera(plr);

	// if (Event::MousePressed(SDLK_MIDDLEMOUSE))
	// {
	// 	int x, y;
	// 	SDL_GetMouseState(&x, &y);
	// 	window.cameraPos = Vector2(x - offsetMouse.x + offsetCam.x, y - offsetMouse.y + offsetCam.y);
	// }
	// else if(!Event::MousePressed(SDLK_MIDDLEMOUSE))
	// {
	// 	int x, y;
	// 	SDL_GetMouseState(&x, &y);
	// 	offsetMouse = Vector2(x, y);
	// 	offsetCam = window.cameraPos;
	// }

	// if (Event::MousePressed(SDLK_LEFTMOUSE)) // for debugging
	// {
	// 	Level::LoadLevel(Level::LoadFile("assets/levels/level1.lvl"), walls, window, tileSet);
	// 	// int x;
	// 	// int y;
	// 	// SDL_GetMouseState(&x, &y);
	// 	// plr.setPos(Vector2(x, y));
	// }
	
}

void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
	plr.update();
	
	for (Entity wall : walls)
	{
		plr.getCol(wall);
	}
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
    	Event::PollEvent();
    	gameRunning = Event::AppQuit();
		gameLoop();
    	render();
		SDL_Delay(16);
	}

	window.quit(); // run when user asks to exit program
	return 0;
}