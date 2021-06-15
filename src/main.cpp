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

// textures
SDL_Texture* playerTex;

std::vector<Entity> walls; // literally just walls (for the level) (also why the fuck don't i make a seperete entity derived class for the level??? ahh fuck it)

bool load = init(); // this is the end of textures and windows OK NVM

Player plr (Vector2(100, 0), playerTex, Vector2(64, 64));

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window.create("SDL Game", SCREEN_WIDTH, SCREEN_HEIGHT); // name and size of application window

	//textures
	playerTex = window.loadTexture("assets/textures/player.png"); // the texture used for the player

	Level::LoadLevel("assets/levels/level1.lvl", walls, window);

	return true;
}

void gameLoop() // it runs forever
{
	window.camera(plr);


	if (Event::MousePressed(SDLK_LEFTMOUSE)) // for debugging
	{
		int x;
		int y;
		SDL_GetMouseState(&x, &y);
		plr.setPos(Vector2(x, y));
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