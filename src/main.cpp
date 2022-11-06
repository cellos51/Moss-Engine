#include <SDL2/SDL.h>
#include <glad/glad.h>
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
#include "light.hpp"

// random shit needed to be here to run
bool gameRunning = true;

// main window
RenderWindow window;

//fonts
TTF_Font* swansea;

// literally just walls (for the level) (also why the fuck don't i make a seperete entity derived class for the level??? ahh fuck it)
std::vector<Entity> walls; 

// player stuff
Vector2 PlayerSpawn = Vector2(0,0);
Player plr (PlayerSpawn, -1, Vector2(16, 16));

// for online mode
std::map<int,Entity> players;
int netMode = 0;

// user interface test stuff
std::vector<ui::Button> buttons;
ui::TextInput ipInput;
int menuType = 0;

//Vector2 cameraPos;
Light realLight(Vector2(144,144));
std::vector<Light> lights;

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	//IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	SDL_StopTextInput();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

	if (enet_initialize () != 0)
    {
        std::cout << "An error occurred while initializing ENet.\n";
        return EXIT_FAILURE;
    }

	window.create("Moss Engine", 1280, 720); // name and size of application window

	//fonts
	swansea = TTF_OpenFont("assets/fonts/swansea.ttf", 40);

	ipInput.uiText.font = swansea;

	plr.setTex(window.loadTexture("assets/textures/light_animsheet.png"));



	plr.transform = Level::LoadLevel(Level::LoadFile("assets/levels/level.lvl"), walls, window);
	plr.layer = 2;

	//window.loadTexture(""); // major bullshit but this is like the tf2 coconut kinda

	realLight.layer = 2;
	realLight.intensity = 1;

	return true;
}

void gameLoop() // it runs forever
{
	if (Event::KeyDown(SDLK_ESCAPE))
	{
		if (menuType > 0)
		{
			menuType = 0;
			buttons.clear();
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
			ui::Button buttonz;
			buttonz.uiText.font = swansea;
			for (int i = 0; i < 3; i++)
			{
				buttons.push_back(buttonz);
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
			ui::Button buttonz;
			buttonz.uiText.font = swansea;
			for (int i = 0; i < 3; i++)
			{
				buttons.push_back(buttonz);
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
			ui::Button buttonz;
			buttonz.uiText.font = swansea;
			for (int i = 0; i < 2; i++)
			{
				buttons.push_back(buttonz);
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
			menuType = 2; 
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
					Entity tempPlr (window.loadTexture("assets/textures/player.png"), Vector2(16,16));
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
					Entity tempPlr (window.loadTexture("assets/textures/player.png"), Vector2(16,16));
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

	if (Event::KeyPressed(SDLK_1))
	{
		//window.zoom = 0;
		window.setZoom(1);

	}
	else if (Event::KeyPressed(SDLK_2))
	{
		//window.zoom = 0.25;
		window.setZoom(2);
	}
	else if (Event::KeyPressed(SDLK_3))
	{
		//window.zoom = 0.5;
		window.setZoom(3);
	}
	else if (Event::KeyPressed(SDLK_4))
	{
		//window.zoom = 0.75;
		window.setZoom(4);
	}	

	plr.update();
	window.camera(Vector2(plr.transform.x + plr.size.x / 2, plr.transform.y + plr.size.y / 2));
	//window.camera(cameraPos);

	for (Entity wall : walls)
	{
		plr.getCol(wall);
	}

	int x, y;
	SDL_GetMouseState(&x, &y);

	x = ((x + (window.cameraPos.x)) - window.getSize().x / 2);
	y = ((y + (window.cameraPos.y)) - window.getSize().y / 2);

	realLight.transform = Vector2(x,y);

	if (Event::MouseDown(SDL_BUTTON_RIGHT))
	{
		//for (int i = 0; i < 100; i++)
		//{
		lights.push_back(realLight);
		std::cout << "Lights: " << lights.size() + 1 << std::endl;
		//}
	}

	if (Event::KeyDown(SDLK_q))
	{
		realLight.radius += 10;
	}
	else if (Event::KeyDown(SDLK_e))
	{
		realLight.radius -= 10;
	}
}


void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
  	// for (std::map<int,Entity>::iterator it = players.begin(); it != players.end(); ++it)
  	// {
  	// 	window.render(it->second, true);
  	// }

	window.render(realLight);

	for (Light light : lights)
	{
		window.render(light);
	}

	for (Entity wall : walls)
	{
		window.render(wall, true);
	}

	window.render(plr, true);

	
	if (menuType > 0)
	{
		for (ui::Button button : buttons)
		{
			window.render(button);
		}
	}
	
	// if (menuType == 3)
	// {
	// 	window.render(ipInput);
	// }
}

int main(int argc, char* args[])
{
	init();
	while (gameRunning) // main game loop ran every frame
	{
		Time::Tick();
    	Event::PollEvent();
    	gameRunning = Event::AppQuit();
		gameLoop();

		window.clear();
    	render();
    	window.display();
	}

	atexit (enet_deinitialize);
	window.quit(); // run when user asks to exit program
	return 0;
}