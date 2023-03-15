#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <vector>
#include <iostream>
#include <map>

#include "network.hpp"
#include "text.hpp"
#include "math.hpp"
#include "openglwindow.hpp"
#include "vulkanwindow.hpp"
#include "player.hpp"
#include "event.hpp"
#include "level.hpp"
#include "ui.hpp"
#include "light.hpp"

// random shit needed to be here to run
bool gameRunning = true;

// main window
OpenGLWindow window;
//VulkanWindow window;

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

// gravity test thing
PhysicsEntity boi(Vector2(16,16));

bool init() // used to initiate things before using
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_StopTextInput();

	// if (enet_initialize () != 0)
    // {
    //     std::cout << "An error occurred while initializing ENet.\n";
    //     return EXIT_FAILURE;
    // }

	window.create("Moss Engine", 1280, 720); // name and size of application window

	plr.transform = Level::LoadLevel(Level::LoadFile("assets/levels/level.lvl"), walls, window);
	plr.setTex(window.loadTexture("assets/textures/light_animsheet.png"));
	plr.layer = 3;

	realLight.layer = 2;
	realLight.intensity = 1;

	FPS.font = window.loadTexture("assets/fonts/font.png");
	FPS.setText("FPS");

	boi.setTex(window.loadTexture("assets/textures/player.png"));
	boi.layer = 3;
	boi.gravity = Vector2(0,0);
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


	boi.velocity.x += ((plr.transform.x - boi.transform.x) / 1000) * Time::deltaTime();
	boi.velocity.y += ((plr.transform.y - boi.transform.y)  / 1000) * Time::deltaTime();
	boi.update();
	//window.camera(cameraPos);

	for (Entity wall : walls)
	{
		plr.getCol(wall);
		boi.getCol(wall);
	}

	// int x, y;
	// SDL_GetMouseState(&x, &y);

	// x = ((x + (window.cameraPos.x)) - window.getSize().x / 2);
	// y = ((y + (window.cameraPos.y)) - window.getSize().y / 2);

	realLight.transform = Vector2(plr.transform.x + plr.size.x / 2, plr.transform.y + plr.size.y / 2);

	// if (Event::MouseDown(SDL_BUTTON_RIGHT))
	// {
	// 	//for (int i = 0; i < 100; i++)
	// 	//{
	// 	lights.push_back(realLight);
	// 	std::cout << "Lights: " << lights.size() + 1 << std::endl;
	// 	//}
	// }

	// if (Event::KeyDown(SDLK_q))
	// {
	// 	realLight.radius += 10;
	// }
	// else if (Event::KeyDown(SDLK_e))
	// {
	// 	realLight.radius -= 10;
	// }
	// else if (Event::KeyDown(SDLK_r))
	// {
	// 	std::cout << std::endl << "Red: ";
	// 	std::cin >> realLight.r;
	// 	std::cout << std::endl << "Green: ";
	// 	std::cin >> realLight.g;
	// 	std::cout << std::endl << "Blue: ";
	// 	std::cin >> realLight.b;
	// }
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

	window.render(boi, true);
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