#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <iostream>
#include <tuple>

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
SDL_Texture* tileSet[15];
SDL_Texture* tileSetAplha[15];

std::vector<Entity> walls; // literally just walls (for the level) (also why the fuck don't i make a seperete entity derived class for the level??? ahh fuck it)

// width height of level (defualt: 16:10)
int LVLwidth = 16;
int LVLheight = 10;

inline bool isInteger(const std::string & s) // yeah i just fucking copied this from stackoverflow credits to paercebal i think
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

bool load = init(); // this is the end of textures and windows OK NVM

//Player plr (Vector2(100, 0), playerTex, Vector2(64, 64));

unsigned cursorTexId = 0;
Entity cursor (Vector2(64, 64), tileSetAplha[cursorTexId]);

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window.create("Moss Level Editor", SCREEN_WIDTH, SCREEN_HEIGHT); // name and size of application window

	// textures
	//playerTex = window.loadTexture("assets/textures/player.png"); // the texture used for the player
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
	tileSet[14] = window.loadTexture("assets/textures/player.png");
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
	tileSetAplha[14] = window.loadTexture("assets/textures/player.png");

	for (unsigned i = 0; i < sizeof tileSetAplha/sizeof tileSetAplha[0] + 1; i++)
	{
		SDL_SetTextureAlphaMod(tileSetAplha[i], 128);
	}

	std::cout << "Welcome to Moss Level Editor." << std::endl;
	std::cout << "Would you like to load a level or create a new one? (load/new)" << std::endl;

	while (true)
	{
		std::string input;
		std::cin >> input;
		if (input == "load")
		{
			std::cout << std::endl << "Please input the name of the level you wish to load (level1)" << std::endl;
			std::cin >> input;
			std::cout << std::endl << "Loading..." << std::endl;
			std::tie(LVLheight, LVLwidth) = Level::LoadLevel(Level::LoadFile("assets/levels/" + input + ".lvl"), walls, window, tileSet); // i'd rather be lonely than tied to a phase
			std::cout << "Loading complete!" << std::endl;
			std::cout << "When you are ready press 'S' to save your level." << std::endl;
			break;
		}
		else if (input == "new")
		{
			std::cout << std::endl << "Please input the width value of the level (16)" << std::endl;
			while (true)
			{
				std::cin >> input;
				if (isInteger(input))
				{
					LVLwidth = std::stoi(input);
					break;	
				}
				else
				{
					std::cout << std::endl << "Error: did not recieve valid int" << std::endl;
				}
			}
			std::cout << std::endl << "Please input the height value of the level (10)" << std::endl;
			while (true)
			{
				std::cin >> input;

				if (isInteger(input))
				{
					LVLheight = std::stoi(input);
					break;	
				}
				else
				{
					std::cout << std::endl << "Error: did not recieve valid int" << std::endl;
				}
			}
			std::cout << "Setup complete." << std::endl << "When you are ready press 'S' to save your level." << std::endl;
			break;
		}
		else
		{
			std::cout << std::endl << "Error: string recieved not (load/new)" << std::endl;
		}
	}

	return true;
}

void LevelCompile (std::vector<Entity>& p_ent, int lvlX, int lvlY)
{
	std::string input;
	std::cout << "Please enter the name you wish the file to be saved as (same names as preesxisting files will be overwritten.)" << std::endl;
	std::cin >> input;
	input = input + ".lvl";

	std::cout << std::endl << "Saving..." << std::endl;
	std::string levelData[lvlX * lvlY];

	for (int i = 0; i < lvlX * lvlY; i++)
	{
		levelData[i] = "0";
	}

	for (Entity wall : p_ent)
	{
		int xpos = wall.getPos().x / 64;
		int ypos = wall.getPos().y / 64;

		for (int y = 0; y < lvlY; y++)
		{
			for (int x = 0; x < lvlX; x++)
			{
				if (x == xpos && y == ypos)
				{
					levelData[x + lvlX * y] = wall.id;
				}	
			}
		}
	}

	for (int i = 0; i < lvlX * lvlY; i++)
	{
		std::cout << levelData[i] << " ";
	}

	Level::SaveLevel("assets/levels/" + input, levelData, lvlX, lvlY);

	std::cout << std::endl << "Saving complete!" << std::endl;
	std::cout << "File saved as: " + input << std::endl;
}

void gameLoop() // it runs forever
{
	if (Event::KeyPressed(SDLK_S))
	{
		LevelCompile(walls, LVLwidth, LVLheight);
	}
	//window.camera(plr);

	int x, y;
	SDL_GetMouseState(&x, &y);

	int xf = (x  - window.cameraPos.x) / 64;
	int yf = (y  - window.cameraPos.y) / 64;

	if(Vector2(xf * 64, yf * 64).x < 0 || Vector2(xf * 64, yf * 64).y < 0)
	{	
	}
	else if (Vector2(xf * 64, yf * 64).x < LVLwidth * 64 && Vector2(xf * 64, yf * 64).y < LVLheight * 64)
	{
		cursor.setPos(Vector2(xf * 64, yf * 64));
	}

	cursor.setTex(tileSetAplha[Event::MouseWheel()]);

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
			Entity tile (cursor.getPos(), tileSet[Event::MouseWheel()], Vector2(64, 64));
			tile.id = std::to_string(Event::MouseWheel() + 1);
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
	
	window.render(cursor, true);

	for (Entity wall : walls)
	{
		window.render(wall, true);
	}

	for (int y = 0; y < LVLheight + 1; y++)
	{
		window.drawLine(0, y * 64, LVLwidth * 64, y * 64, true);
	}
	for (int x = 0; x < LVLwidth + 1; x++)
	{
		window.drawLine(x * 64, 0, x * 64, LVLheight * 64, true);
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
		//SDL_Delay(16);
	}

	window.quit(); // run when user asks to exit program
	return 0;
}