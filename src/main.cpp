#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <map>

#include "text.hpp"
#include "math.hpp"

#ifdef OPENGL
#include "openglwindow.hpp"
#elif VULKAN
#include "vulkanwindow.hpp"
#endif

#include "player.hpp"
#include "event.hpp"
#include "level.hpp"
#include "ui.hpp"
#include "light.hpp"

// random shit needed to be here to run
bool gameRunning = true;

// main window
#ifdef OPENGL
OpenGLWindow window;
#elif VULKAN
VulkanWindow window;
#endif

// literally just walls (for the level) (also why the fuck don't i make a seperete entity derived class for the level??? ahh fuck it)
std::vector<Entity> walls; 

// player stuff
Vector2 PlayerSpawn = Vector2(0,0);
Player plr (PlayerSpawn, -1, Vector2(16, 16));

// for online mode
std::map<int,Entity> players;
int netMode = 0;

// Vector2 cameraPos;
Light realLight(Vector2(144,144));
std::vector<Light> lights;

// fps counter
float timer = 0;
Text FPS;

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_StopTextInput();

	#ifdef OPENGL
	window.create("Moss Engine (OpenGL)", 1280, 720); // name and size of application window
	#elif VULKAN
	window.create("Moss Engine (Vulkan)", 1280, 720); // name and size of application window
	#endif

	plr.transform = Level::LoadLevel(Level::LoadFile("assets/levels/level.lvl"), walls, window);
	plr.setTex(window.loadTexture("assets/textures/light_animsheet.png"));
	plr.layer = 3;

	realLight.layer = 2;
	realLight.intensity = 1.5;
	realLight.radius = 150;

	FPS.font = window.loadTexture("assets/fonts/font.png");
	FPS.setText("FPS");
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

	plr.update();
	window.camera(Vector2(plr.transform.x + plr.size.x / 2, plr.transform.y + plr.size.y / 2));

	//window.camera(cameraPos);

	for (Entity wall : walls)
	{
		plr.getCol(wall);
	}

	realLight.transform = Vector2(plr.transform.x + plr.size.x / 2, plr.transform.y + plr.size.y / 2);
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

	window.render(FPS, false);
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
	window.quit(); // run when user asks to exit program
	return 0;
}