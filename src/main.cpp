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

#include "ui.hpp"

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

// for online mode
std::map<int,Entity> players;
int netMode = 0;

// user interface test stuff
std::vector<ui::Button> buttons;
ui::Button button; // this stupid thing has to be here because if i put it in the same loop as the vector it crashes
ui::TextInput ipInput;
int menuType = 0;

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	SDL_StopTextInput();

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
	if (Event::KeyDown(SDLK_ESCAPE))
	{
		if (menuType > 0)
		{
			menuType = 0;
		}
		else if (menuType == 0)
		{
			menuType = 1;
		}
	}

	if (menuType == 1)
	{
		if (buttons.size() != 3)
		{
			buttons.clear();
			for (int i = 0; i < 3; i++)
			{
				buttons.push_back(button);
			}
		}

		buttons[0].size.x = 200;
		buttons[0].size.y = 64;

		buttons[0].transform.x = (window.getSize().x / 2) - (buttons[0].size.x / 2);
		buttons[0].transform.y = (window.getSize().y / 2) - 200;
		buttons[0].uiText.setText("Resume");

		buttons[1].size.x = 200;
		buttons[1].size.y = 64;

		buttons[1].transform.x = (window.getSize().x / 2) - (buttons[0].size.x / 2);
		buttons[1].transform.y = (window.getSize().y / 2) - 100;
		buttons[1].uiText.setText("Multiplayer");

		buttons[2].size.x = 200;
		buttons[2].size.y = 64;

		buttons[2].transform.x = (window.getSize().x / 2) - (buttons[0].size.x / 2);
		buttons[2].transform.y = (window.getSize().y / 2);
		buttons[2].uiText.setText("Quit");


		//button.uiText.transform.x = (button.transform.x) + ((button.size.x / 2) - button.uiText.size.x / 2);
		//button.uiText.transform.y = (button.transform.y) + ((button.size.y / 2) - button.uiText.size.x / 2);

		if (buttons[0].onClick())
		{
			menuType = 0; 
		}

		if (buttons[1].onClick())
		{
			menuType = 2; 
		}

		if (buttons[2].onClick())
		{
			gameRunning = false; 
		}
	}
	else if (menuType == 2)
	{
		if (buttons.size() != 3)
		{
			buttons.clear();
			for (int i = 0; i < 3; i++)
			{
				buttons.push_back(button);
			}
		}

		buttons[0].size.x = 200;
		buttons[0].size.y = 64;

		buttons[0].transform.x = (window.getSize().x / 2) - (buttons[0].size.x / 2);
		buttons[0].transform.y = (window.getSize().y / 2) - 200;
		buttons[0].uiText.setText("Host");

		buttons[1].size.x = 200;
		buttons[1].size.y = 64;

		buttons[1].transform.x = (window.getSize().x / 2) - (buttons[0].size.x / 2);
		buttons[1].transform.y = (window.getSize().y / 2) - 100;
		buttons[1].uiText.setText("Join");

		buttons[2].size.x = 200;
		buttons[2].size.y = 64;

		buttons[2].transform.x = (window.getSize().x / 2) - (buttons[0].size.x / 2);
		buttons[2].transform.y = (window.getSize().y / 2);
		buttons[2].uiText.setText("Back");


		if (buttons[0].onClick())
		{
			Net::serverCreate();
			netMode = 1;
			menuType = 0; 
		}

		if (buttons[1].onClick())
		{
			menuType = 3; 
		}

		if (buttons[2].onClick())
		{
			menuType = 1; 
		}
	}
	else if (menuType == 3)
	{
		if (buttons.size() != 2)
		{
			buttons.clear();
			for (int i = 0; i < 2; i++)
			{
				buttons.push_back(button);
			}
		}

		ipInput.size.x = 400;
		ipInput.size.y = 64;

		ipInput.transform.x = (window.getSize().x / 2) - (ipInput.size.x / 2);
		ipInput.transform.y = (window.getSize().y / 2) - 200;
		ipInput.startTextInput();

		buttons[0].size.x = 200;
		buttons[0].size.y = 64;

		buttons[0].transform.x = (window.getSize().x / 2) - (buttons[0].size.x / 2);
		buttons[0].transform.y = (window.getSize().y / 2) - 100;
		buttons[0].uiText.setText("Connect");

		buttons[1].size.x = 200;
		buttons[1].size.y = 64;

		buttons[1].transform.x = (window.getSize().x / 2) - (buttons[0].size.x / 2);
		buttons[1].transform.y = (window.getSize().y / 2);
		buttons[1].uiText.setText("Back");

		if (buttons[0].onClick())
		{
			Net::clientConnect(ipInput.uiText.getText());
			netMode = 2; 
			menuType = 0;
		}

		if (buttons[1].onClick())
		{
			menuType = 1; 
		}
	}

	if (netMode == 1)
	{
		Net::poll();
		Net::sendPacket(plr.transform);

		if (players.count(Net::getId()))
		{
			Vector2 futurePos = Net::getPacket();

			players.at(Net::getId()).transform.x = std::lerp(players.at(Net::getId()).transform.x, futurePos.x, 0.1);
			players.at(Net::getId()).transform.y = std::lerp(players.at(Net::getId()).transform.y, futurePos.y, 0.1);
		}

		if (players.size() != Net::allPlayers().size())
		{
			for (int ids : Net::allPlayers())
			{
				if (!players.count(ids))
				{
					Entity tempPlr (window.loadTexture("assets/textures/player.png"), Vector2(64,64));
					players.insert({ids, tempPlr});
				}	
			}

			std::map<int,Entity> copy = players;
			for (std::map<int,Entity>::iterator it = copy.begin(); it != copy.end(); ++it)
			{
				bool hasID = false;
				for (int ids : Net::allPlayers())
				{
					if (it->first == ids)
					{
						hasID = true;
					}
				}
				if (hasID == false)
				{
					players.erase(it->first);
				}
			}
		}
	}
	else if (netMode == 2)
	{
		Net::poll();
		Net::sendPacket(plr.transform);

		if (players.count(Net::getId()))
		{
			Vector2 futurePos = Net::getPacket();

			players.at(Net::getId()).transform.x = std::lerp(players.at(Net::getId()).transform.x, futurePos.x, 0.1);
			players.at(Net::getId()).transform.y = std::lerp(players.at(Net::getId()).transform.y, futurePos.y, 0.1);
		}

		if (players.size() != Net::allPlayers().size())
		{
			for (int ids : Net::allPlayers())
			{
				if (!players.count(ids))
				{
					Entity tempPlr (window.loadTexture("assets/textures/player.png"), Vector2(64,64));
					players.insert({ids, tempPlr});
				}	
			}

			std::map<int,Entity> copy = players;
			for (std::map<int,Entity>::iterator it = copy.begin(); it != copy.end(); ++it)
			{
				bool hasID = false;
				for (int ids : Net::allPlayers())
				{
					if (it->first == ids)
					{
						hasID = true;
					}
				}
				if (hasID == false)
				{
					players.erase(it->first);
				}
			}
		}
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

	if (menuType > 0)
	{
		for (ui::Button button : buttons)
		{
			window.render(button);
		}
	}

	if (menuType == 3)
	{
		window.render(ipInput);
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

	atexit (enet_deinitialize);
	window.quit(); // run when user asks to exit program
	return 0;
}