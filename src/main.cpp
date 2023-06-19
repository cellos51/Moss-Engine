#include <steam/steam_api.h>
#include <steam/isteammatchmaking.h>
#include <steam/isteamfriends.h>
#include "networkmanager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <map>

#include "text.hpp"
#include "math.hpp"

#include "openglwindow.hpp"

#include "global.hpp"

#include "player.hpp" // TODO make it so everytime a new type of entity is creted i don't have to #include it :(
#include "enemy.hpp"

#include "event.hpp"
#include "level.hpp"
#include "ui.hpp"
#include "light.hpp"

// random shit needed to be here to run
bool gameRunning = true;
NetworkManager netManager;

// main window
OpenGLWindow window;

// literally just walls (for the level) (also why the fuck don't i make a seperete entity derived class for the level??? ahh fuck it)
std::vector<Entity> walls; 

// player stuff
std::map<unsigned int,LivingEntity*> LivingEnts;

// Vector2 cameraPos;
Light realLight(Vector2(144,144));
std::vector<Light> lights;

// fps counter
float timer = 0;
Text FPS;

Mix_Music *music = NULL;

bool init() // used to initiate things before using
{
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) // Replace with your App ID
	{
		gameRunning = false;
	}

	if (!SteamAPI_Init())
	{
		printf("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n");
		gameRunning = false;
	}

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        gameRunning = false;
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		gameRunning = false;
	}

	SDL_StopTextInput();

	window.create("Moss Engine (OpenGL)", 1280, 720); // name and size of application window
	unsigned int playerTex = window.loadTexture("assets/textures/light_animsheet.png");
	unsigned int enemyTex = window.loadTexture("assets/textures/player.png");

	LivingEnts.emplace(0 ,new Player (Vector2(0,0), -1, Vector2(16, 16)));

	LivingEnts[0]->transform = Level::LoadLevel(Level::LoadFile("assets/levels/level.lvl"), walls, window);
	LivingEnts[0]->setTex(playerTex);
	LivingEnts[0]->layer = 3;

	LivingEnts.emplace(1 ,new Enemy (Vector2(0,0), -1, Vector2(16, 16)));
	LivingEnts[1]->setTex(enemyTex);
	LivingEnts[1]->layer = 3;

	realLight.layer = 2;
	realLight.intensity = 1.5;
	realLight.radius = 150;

	FPS.font = window.loadTexture("assets/fonts/font.png");
	FPS.setText("FPS");

	music = Mix_LoadMUS("assets/audio/Synchronicity.flac");
	//Mix_PlayMusic(music, -1);

	netManager.playerTex = playerTex;

	return true;
}

void gameLoop() // it runs forever
{
	timer += Time::deltaTime();

	if (timer > 100)
	{
		FPS.setText("FPS: " + std::to_string(int(1000 / Time::deltaTime())));
		timer = 0;
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

	for (auto& [key, ent]: LivingEnts)
	{
		ent->update();
	}

	window.camera(Vector2(LivingEnts[0]->transform.x + LivingEnts[0]->size.x / 2, LivingEnts[0]->transform.y + LivingEnts[0]->size.y / 2));

	//window.camera(cameraPos);

	for (Entity wall : walls)
	{
		for (auto& [key, ent]: LivingEnts)
		{
			ent->getCol(wall);
		}
	}

	realLight.transform = Vector2(LivingEnts[0]->transform.x + LivingEnts[0]->size.x / 2, LivingEnts[0]->transform.y + LivingEnts[0]->size.y / 2);

	// lobby stuff
	if (Event::KeyDown(SDLK_p))
	{
		netManager.CreateLobby();
	}
	else if (netManager.InLobby())
	{
		// Create a byte array to hold the serialized float values
		uint32_t dataSize = sizeof(PlayerData);
		uint8_t* pData = new uint8_t[dataSize];

		PlayerData data;
		data.index = netManager.playerIndex;
		data.position = LivingEnts[0]->transform;

		if (LivingEnts[0]->movementDir.magnitude() > 0)
		{
			if (LivingEnts[0]->running == true)
			{
				data.movement = 2;
			}
			else
			{
				data.movement = 1;
			}

			if (LivingEnts[0]->movementDir.y > 0)
			{
				data.direction = 4;
			}
			else if (LivingEnts[0]->movementDir.y < 0)
			{
				data.direction = 3;
			}
			else if (LivingEnts[0]->movementDir.x < 0)
			{
				data.direction = 2;
			}
			else if (LivingEnts[0]->movementDir.x > 0)
			{
				data.direction = 1;
			}
		}
		else
		{
			data.movement = 0;
			data.direction = 0;
		}
		


		// Serialize the float values into the byte array
		memcpy(pData, &data, dataSize);

		netManager.MessageAll(pData, dataSize, k_nSteamNetworkingSend_UnreliableNoDelay, 0);
	}

}

void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
  	// for (std::map<int,Entity>::iterator it = LivingEnts.begin(); it != LivingEnts.end(); ++it)
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

	for (auto& [key, value] : netManager.netPlayers)
	{
		value.update();
		window.render(value, true);
	}

	for (auto& [key, ent]: LivingEnts)
	{
		window.render(*ent, true);
	}

	window.render(LivingEnts[0]->hitBox, true);

	window.render(FPS, false);
}

int main(int argc, char* args[])
{
	init();
	while (gameRunning) // main game loop ran every frame
	{
		SteamAPI_RunCallbacks();
		netManager.ReceiveMessages();

		Time::Tick();
    	Event::PollEvent();
    	gameRunning = Event::AppQuit();
		gameLoop();

		window.clear();
    	render();
    	window.display();
	}

	for (auto& [key, ent]: LivingEnts)
	{
		delete ent;
	}
	LivingEnts.clear();
	
	window.quit(); // run when user asks to exit program
	Mix_Quit();
	SDL_Quit();
	SteamAPI_Shutdown();
	return 0;
}