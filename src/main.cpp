#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>

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
SDL_Texture* tileSetAplha[13];

std::vector<Entity> walls; // literally just walls (for the level) (also why the fuck don't i make a seperete entity derived class for the level??? ahh fuck it)

bool load = init(); // this is the end of textures and windows OK NVM

//Player plr (Vector2(100, 0), playerTex, Vector2(64, 64));

Entity cursor (Vector2(64, 64), tileSetAplha[0]);

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
	tileSetAplha[0] = window.loadTexture("assets/textures/grass.png"); // ok if you are browsing my code on github to learn. PLEASE. DO. NOT. DO. THIS. it is very bad and im only doing this because this isn't very important.
	tileSetAplha[1] = window.loadTexture("assets/textures/dirt1.png");
	tileSetAplha[2] = window.loadTexture("assets/textures/dirt2.png");
	tileSetAplha[3] = window.loadTexture("assets/textures/dirt3.png");
	tileSetAplha[4] = window.loadTexture("assets/textures/dirt4.png");
	tileSetAplha[5] = window.loadTexture("assets/textures/dirt5.png");
	tileSetAplha[6] = window.loadTexture("assets/textures/dirt6.png");
	tileSetAplha[7] = window.loadTexture("assets/textures/dirt7.png");
	tileSetAplha[8] = window.loadTexture("assets/textures/dirt8.png"); 
	tileSetAplha[9] = window.loadTexture("assets/textures/dirt9.png");
	tileSetAplha[10] = window.loadTexture("assets/textures/dirt10.png");
	tileSetAplha[11] = window.loadTexture("assets/textures/dirt11.png");
	tileSetAplha[12] = window.loadTexture("assets/textures/dirt12.png");
	tileSetAplha[13] = window.loadTexture("assets/textures/dirt13.png");

	for (unsigned i = 0; i < sizeof tileSetAplha/sizeof tileSetAplha[0]; i++)
	{
		SDL_SetTextureAlphaMod(tileSetAplha[i], 128);
	}

	Level::LoadLevel(Level::LoadFile("assets/levels/level1.lvl"), walls, window, tileSet); // i'd rather be lonely than tied to a phase

	int width = 16;
	int height = 10;

	std::string levelData[width * height];

	for (int i = 0; i < width * height; i++)
	{
		levelData[i] = "0";
	}

	for (Entity wall : walls)
	{
		int xpos = wall.getPos().x / 64;
		int ypos = wall.getPos().y / 64;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (x == xpos && y == ypos)
				{
					levelData[x + width * y] = wall.id;
				}	
			}
		}
	}

	for (int i = 0; i < width * height; i++)
	{
		std::cout << levelData[i] << " ";
	}

	Level::SaveLevel("assets/levels/test.lvl", levelData, width, height);

	return true;
}

void gameLoop() // it runs forever
{
	//window.camera(plr);

	int x, y;
	SDL_GetMouseState(&x, &y);

	int xf = (x  - window.cameraPos.x) / (SCREEN_WIDTH / 16);
	int yf = (y  - window.cameraPos.y) / (SCREEN_HEIGHT / 10);

	if(Vector2(xf * (SCREEN_WIDTH / 16), yf * (SCREEN_HEIGHT / 10)).x < 0 || Vector2(xf * (SCREEN_WIDTH / 16), yf * (SCREEN_HEIGHT / 10)).y < 0)
	{	
	}
	else if (Vector2(xf * (SCREEN_WIDTH / 16), yf * (SCREEN_HEIGHT / 10)).x < 16 * 64 && Vector2(xf * (SCREEN_WIDTH / 16), yf * (SCREEN_HEIGHT / 10)).y < 10 * 64)
	{
		cursor.setPos(Vector2(xf * (SCREEN_WIDTH / 16), yf * (SCREEN_HEIGHT / 10)));
	}



	if (Event::MousePressed(SDLK_MIDDLEMOUSE))
	{
		window.cameraPos = Vector2(x - offsetMouse.x + offsetCam.x, y - offsetMouse.y + offsetCam.y);
	}
	else if(!Event::MousePressed(SDLK_MIDDLEMOUSE))
	{
		offsetMouse = Vector2(x, y);
		offsetCam = window.cameraPos;
	}

	if (Event::MousePressed(SDLK_LEFTMOUSE))
	{
		bool obstructed = false;
		for (Entity wall : walls)
		{
			if (wall.getPos().x == cursor.getPos().x && wall.getPos().y == cursor.getPos().y)
			{
				obstructed = true;
			}
		}
		if (obstructed == false)
		{
			Entity tile (cursor.getPos(), tileSet[0], Vector2(64, 64));
			walls.push_back(tile);
		}
	}
	else if (Event::MousePressed(SDLK_RIGHTMOUSE))
	{
		for (unsigned i = 0; i < walls.size(); i++)
		{
			if (walls[i].getPos().x == cursor.getPos().x && walls[i].getPos().y == cursor.getPos().y)
			{
				walls.erase(walls.begin() + i);
			}
		}
	}

}

void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
	//plr.update();
	
	// for (Entity wall : walls)
	// {
	// 	plr.getCol(wall);
	// }
	window.clear();
	//window.render(plr, true);

	for (Entity wall : walls)
	{
		window.render(wall, true);
	}
	window.render(cursor, true);
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