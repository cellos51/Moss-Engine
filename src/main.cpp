#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

#include "renderwindow.hpp"

#include "player.hpp"

#include "event.hpp"

#include "level.hpp"

const short SCREEN_WIDTH = 1024;
const short SCREEN_HEIGHT = 640;

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

	if (Event::KeyPressed(SDLK_UPARROW))
	{
		plr.velocity.y = -20;
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