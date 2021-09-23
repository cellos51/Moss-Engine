#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>
#include <map>

#include "network.hpp"

#include "text.hpp"

#include "math.hpp"

#include "renderwindow.hpp"

#include "player.hpp"

#include "event.hpp"

#include "level.hpp"

#include "config.hpp" // this just has the screen size so if i change it i dont need to change it in every class

// random shit needed to be here to run
bool init();
bool gameRunning = true;

//networking
Server cp_supplier;
Client cp_buyer;
int bruh = 0;

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
std::map<int,Entity> players; // for online mode


Text fps; // simple fps counter (enabling this literally halves the performance wtf. i should really use that guys cached font c library)

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	if (enet_initialize () != 0)
    {
        std::cout << "An error occurred while initializing ENet.\n";
        return EXIT_FAILURE;
    }

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
	if (Event::KeyPressed(SDLK_j) && bruh == 0)
	{
		cp_supplier.create();
		bruh = 1;
	}
	else if (Event::KeyPressed(SDLK_k) && bruh == 0)
	{
		cp_buyer.connect();
		bruh = 2;
	}
	else if (bruh == 1)
	{
		cp_supplier.poll();
		cp_supplier.sendPacket(plr.transform);
		if (!players.count(cp_supplier.getId()))
		{
			Entity tempPlr (window.loadTexture("assets/textures/player.png"), Vector2(64,64));
			players.insert({cp_supplier.getId(), tempPlr});
		}
		players.at(cp_supplier.getId()).transform = cp_supplier.getPacket();
	}
	else if (bruh == 2)
	{
		cp_buyer.poll();
		cp_buyer.sendPacket(plr.transform);
		if (!players.count(cp_buyer.getId()))
		{
			Entity tempPlr (window.loadTexture("assets/textures/player.png"), Vector2(64,64));
			players.insert({cp_buyer.getId(), tempPlr});
		}
		players.at(cp_buyer.getId()).transform = cp_buyer.getPacket();
	}





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

	fps.setText("fps: " + std::to_string(1000 / Time::deltaTime()));
}


void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
	window.clear();

  	for (std::map<int,Entity>::iterator it = players.begin(); it != players.end(); ++it)
  	{
  		window.render(it->second, true);
  	}
   	

	window.render(plr, true);

	for (Entity wall : walls)
	{
		window.render(wall, true);
	}
	window.render(fps, false);
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

	atexit (enet_deinitialize);
	window.quit(); // run when user asks to exit program
	return 0;
}