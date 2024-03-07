#include <stddef.h>
#include <WinSock2.h>
#include <steam/steam_api.h>
#include "networking.hpp"

#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <map>
#include <Commdlg.h>
#include <shlwapi.h>

#include "scene.hpp"
#include "splashscreenscene.hpp"
#include "editorscene.hpp"
#include "gamescene.hpp"
#include "console.hpp"
#include "text.hpp"
#include "math.hpp"
#include "openglwindow.hpp"
#include "event.hpp"
#include "level.hpp"
#include "ui.hpp"
#include "light.hpp"

const double fixedTick = 1000.0 / 60.0;
double fixedTime = 0.0;

static bool init() // used to initiate things before using
{
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) // Replace with your App ID
	{
		//console.gameRunning = false;
	}

	if (!SteamAPI_Init())
	{
		console.log("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n");
		//console.gameRunning = false;
	}

	steamSocket.init();

	if( SDL_Init( SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER ) < 0 )
    {
        console.log( "SDL could not initialize! SDL Error: " + std::string(SDL_GetError()) + "\n");
        console.gameRunning = false;
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		console.log( "SDL_mixer could not initialize! SDL_mixer Error: " + std::string(Mix_GetError()) + "\n");
		console.gameRunning = false;
	}

	SDL_StopTextInput();

	window.create("Moss Engine (OpenGL)", 1280, 720); // name and size of application window
	console.log("Window creation and program initialization finished.\n");

	activeScene.openScene(std::make_shared<SplashScene>(window));

	// bandaid solution

	window.loadTexture("assets/textures/tile_01.png");
	window.loadTexture("assets/textures/playerstart.png");
	window.loadTexture("assets/textures/selector.png");
	window.loadTexture("assets/textures/buttons.png");
	window.loadTexture("assets/textures/collision.png");
	window.loadTexture("assets/textures/player.png");
	window.loadTexture("assets/textures/splash.png");

	// end of bandaid solution

	console.init();

	Event::AppStart();



	return true;
}

static void gameLoop() // it runs forever	
{
	activeScene.update();
	console.update(); // grrr i have to reference window here because i couldn't include a reference variable cause no constructor :(
}

static void fixedGameLoop() // it runs forever
{
	activeScene.fixedUpdate();
	console.fixedUpdate();
}

static void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
	activeScene.render(window);
	console.render(window);
}

int main(int argc, char* args[])
{
	init();
	while (console.gameRunning) // main game loop ran every frame
	{
		SteamAPI_RunCallbacks();

		Time::Tick();
    	Event::PollEvent();
    	console.gameRunning = Event::AppQuit();
		gameLoop();

		fixedTime += Time::deltaTime();

		while (fixedTime > fixedTick) // mid way to do this but whatevs
		{
			fixedGameLoop();
			fixedTime -= fixedTick;
		}

		window.clear();
    	render();
    	window.display();
	}
	activeScene.closeScene();

	steamSocket.disconnect();
	
	window.quit(); // run when user asks to exit program
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
	SteamAPI_Shutdown();
	return 0;
}