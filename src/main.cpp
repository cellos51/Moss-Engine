#include <steam/steam_api.h>
#include <steam/isteammatchmaking.h>
#include <steam/isteamfriends.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <map>
#include <windows.h>
#include <Commdlg.h>
#include <shlwapi.h>

#include "global.hpp"
#include "scene.hpp"
#include "editorscene.hpp"
#include "text.hpp"
#include "math.hpp"
#include "openglwindow.hpp"
#include "event.hpp"
#include "level.hpp"
#include "ui.hpp"
#include "light.hpp"

// this controls the game loop
bool gameRunning = true;

// main window
OpenGLWindow window;

// main scene
SceneManager mainScene;

bool init() // used to initiate things before using
{
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) // Replace with your App ID
	{
		//gameRunning = false;
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

	mainScene.openScene(std::make_shared<EditorScene>(window));

	return true;
}

void gameLoop() // it runs forever
{
	mainScene.update();
}

void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
	mainScene.render(window);
}

int main(int argc, char* args[])
{
	init();
	while (gameRunning) // main game loop ran every frame
	{
		SteamAPI_RunCallbacks();

		Time::Tick();
    	Event::PollEvent();
    	gameRunning = Event::AppQuit();
		gameLoop();

		window.clear();
    	render();
    	window.display();
	}

	mainScene.closeScene();
	
	window.quit(); // run when user asks to exit program
	Mix_Quit();
	SDL_Quit();
	SteamAPI_Shutdown();
	return 0;
}