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
#include "console.hpp"
#include "text.hpp"
#include "math.hpp"
#include "openglwindow.hpp"
#include "event.hpp"
#include "level.hpp"
#include "ui.hpp"
#include "light.hpp"

// main window
OpenGLWindow window;

// main scene
SceneManager mainScene;

bool init() // used to initiate things before using
{
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) // Replace with your App ID
	{
		//console.gameRunning = false;
	}

	if (!SteamAPI_Init())
	{
		console.log("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n");
		console.gameRunning = false;
	}

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
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


	mainScene.openScene(std::make_shared<EditorScene>(window));

	console.init(window);

	return true;
}

void gameLoop() // it runs forever
{
	mainScene.update();
	console.update(window); // grrr i have to reference window here because i couldn't include a reference variable cause no constructor :(
}

void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
	mainScene.render(window);
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