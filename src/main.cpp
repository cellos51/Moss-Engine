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
#include <windows.h>
#include <Commdlg.h>
#include <shlwapi.h>

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

//player stuff
std::map<unsigned int, LivingEntity*> LivingEnts;

// camra offset used for panning
Vector2 offsetCam;
Vector2 offsetMouse;

// literally just walls (for the level) (also why the fuck don't i make a seperete entity derived class for the level??? ahh fuck it) done lmao (oct 26 '23)
LevelData level;

//std::vector<Entity> walls;
//std::vector<Light> lights; // original name :skull emoji:

std::vector<Entity> collisionView;
unsigned int collisionTexture;
bool viewCollision = false;
bool editingPlayer = false;
bool editingLights = false;

ui::Button tileSet;
ui::Panel topBar;
ui::Panel sideBar;
ui::Button layer1;
ui::Button layer2;
ui::Button layer3;
ui::Button layer4;
ui::Button collisionButton;
ui::Button playerButton;
ui::Button lightsButton;
ui::Button saveButton;
ui::Button saveAsButton;
ui::Button loadButton;
ui::Button importButton;
ui::Slider brightness;
ui::Slider radius;
ui::Slider red;
ui::Slider green;
ui::Slider blue;

unsigned int font = 0;
unsigned int activeLayer = 1;
std::string currentFile;
std::string activeTexture;

Entity selector(Vector2(0, TILE_SIZE * 2), Vector2(TILE_SIZE, TILE_SIZE));
Entity cursor(Vector2(TILE_SIZE, TILE_SIZE));
Light lightCursor;
Entity playerSpawn(Vector2(TILE_SIZE, TILE_SIZE));

// Vector2 cameraPos;

Mix_Music *music = NULL;

void loadTextures(std::string texPath)
{
	if (texPath.length() > 0)
	{
		tileSet.tex = window.loadTexture(texPath.c_str());
	}
}

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

	playerSpawn.tex = window.loadTexture("assets/textures/playerstart.png");
	activeTexture = "assets/textures/tile_01.png";
	tileSet.tex = window.loadTexture(activeTexture.c_str());
	loadTextures(activeTexture);
	cursor.tex = tileSet.tex;
	selector.tex = window.loadTexture("assets/textures/selector.png");
	topBar.tex = window.loadTexture("assets/textures/buttons.png");
	sideBar.tex = topBar.tex;
	layer1.tex = topBar.tex;
	layer2.tex = topBar.tex;
	layer3.tex = topBar.tex;
	layer4.tex = topBar.tex;
	collisionButton.tex = topBar.tex;
	lightsButton.tex = topBar.tex;
	saveButton.tex = topBar.tex;
	saveAsButton.tex = topBar.tex;
	loadButton.tex = topBar.tex;
	playerButton.tex = topBar.tex;
	importButton.tex = topBar.tex;
	collisionTexture = window.loadTexture("assets/textures/collision.png");

	topBar.size = Vector2(window.getSize().x, 48);
	topBar.texturePos.x = 123;
	topBar.texturePos.y = 2;
	topBar.texturePos.w = 44;
	topBar.texturePos.h = 11;
	topBar.transform = Vector2(0, 0);

	//sideBar.size = Vector2(window.getSize().x, 48);
	//sideBar.texturePos.x = 123;
	//sideBar.texturePos.y = 2;
	//sideBar.texturePos.w = 44;
	//sideBar.texturePos.h = 11;
	sideBar.transform = Vector2(0, 0);

	layer1.size = Vector2(26, 26);
	layer1.texturePos.x = 36;
	layer1.texturePos.y = 43;
	layer1.texturePos.w = 13;
	layer1.texturePos.h = 13;
	layer1.transform = Vector2(13, 13);

	layer2.size = Vector2(26, 26);
	layer2.texturePos.x = 36;
	layer2.texturePos.y = 29;
	layer2.texturePos.w = 13;
	layer2.texturePos.h = 13;
	layer2.transform = Vector2(52, 13);

	layer3.size = Vector2(26, 26);
	layer3.texturePos.x = 36;
	layer3.texturePos.y = 15;
	layer3.texturePos.w = 13;
	layer3.texturePos.h = 13;
	layer3.transform = Vector2(91, 13);

	layer4.size = Vector2(26, 26);
	layer4.texturePos.x = 36;
	layer4.texturePos.y = 1;
	layer4.texturePos.w = 13;
	layer4.texturePos.h = 13;
	layer4.transform = Vector2(130, 13);

	collisionButton.size = Vector2(118, 26);
	collisionButton.texturePos.x = 50;
	collisionButton.texturePos.y = 15;
	collisionButton.texturePos.w = 59;
	collisionButton.texturePos.h = 13;
	collisionButton.transform = Vector2(190, 13);

	playerButton.size = Vector2(92, 26);
	playerButton.texturePos.x = 110;
	playerButton.texturePos.y = 15;
	playerButton.texturePos.w = 46;
	playerButton.texturePos.h = 13;
	playerButton.transform = Vector2(330, 13);

	lightsButton.size = Vector2(92, 26);
	lightsButton.texturePos.x = 130;
	lightsButton.texturePos.y = 29;
	lightsButton.texturePos.w = 46;
	lightsButton.texturePos.h = 13;
	lightsButton.transform = Vector2(440, 13);

	saveButton.size = Vector2(64, 26);
	saveButton.texturePos.x = 50;
	saveButton.texturePos.y = 43;
	saveButton.texturePos.w = 32;
	saveButton.texturePos.h = 13;
	saveButton.transform = Vector2(550, 13);

	saveAsButton.size = Vector2(106, 26);
	saveAsButton.texturePos.x = 83;
	saveAsButton.texturePos.y = 43;
	saveAsButton.texturePos.w = 53;
	saveAsButton.texturePos.h = 13;
	saveAsButton.transform = Vector2(630, 13);

	loadButton.size = Vector2(64, 26);
	loadButton.texturePos.x = 50;
	loadButton.texturePos.y = 29;
	loadButton.texturePos.w = 32;
	loadButton.texturePos.h = 13;
	loadButton.transform = Vector2(750, 13);

	importButton.size = Vector2(92, 26);
	importButton.texturePos.x = 83;
	importButton.texturePos.y = 29;
	importButton.texturePos.w = 46;
	importButton.texturePos.h = 13;
	importButton.transform = Vector2(840, 13);

	playerSpawn.layer = 4;
	cursor.layer = 8;
	sideBar.layer = 9;
	tileSet.layer = 10;
	selector.layer = 11;
	topBar.layer = 11;
	layer1.layer = 12;
	layer2.layer = 12;
	layer3.layer = 12;
	layer4.layer = 12;
	collisionButton.layer = 12;
	playerButton.layer = 12;
	lightsButton.layer = 12;
	saveButton.layer = 12;
	saveAsButton.layer = 12;
	loadButton.layer = 12;
	importButton.layer = 12;

	music = Mix_LoadMUS("assets/audio/Synchronicity.flac");
	//Mix_PlayMusic(music, -1);

	window.camera(Vector2(0, 0));
	font = window.loadTexture("assets/fonts/font.png");

	brightness.size = Vector2(64, 16);
	brightness.layer = 13;
	brightness.tex = -1;
	brightness.transform = Vector2(1000, 5);
	brightness.color = Color4(0.0, 0.0, 0.0, 1);
	brightness.uiText.font = font;
	brightness.uiText.transform = Vector2(brightness.transform.x, brightness.transform.y + brightness.size.y);

	radius.size = Vector2(64, 16);
	radius.layer = 13;
	radius.tex = -1;
	radius.transform = Vector2(1000, 22);
	radius.color = Color4(0.5, 0.5, 0.5, 1);
	radius.uiText.font = font;
	radius.uiText.transform = Vector2(radius.transform.x, radius.transform.y + radius.size.y);

	red.size = Vector2(64, 8);
	red.layer = 13;
	red.tex = -1;
	red.transform = Vector2(1100, 5);
	red.color = Color4(0.5, 0.0, 0.0, 1);
	red.uiText.font = font;
	red.uiText.transform = Vector2(red.transform.x, red.transform.y + red.size.y);

	green.size = Vector2(64, 8);
	green.layer = 13;
	green.tex = -1;
	green.transform = Vector2(1100, 16);
	green.color = Color4(0.0, 0.5, 0.0, 1);
	green.uiText.font = font;
	green.uiText.transform = Vector2(green.transform.x, green.transform.y + green.size.y);
	
	blue.size = Vector2(64, 8);
	blue.layer = 13;
	blue.tex = -1;
	blue.transform = Vector2(1100, 27);
	blue.color = Color4(0.0, 0.0, 0.5, 1);
	blue.uiText.font = font;
	blue.uiText.transform = Vector2(blue.transform.x, blue.transform.y + blue.size.y);

	return true;
}

void gameLoop() // it runs forever
{
	brightness.poll();
	radius.poll();
	red.poll();
	green.poll();
	blue.poll();

	tileSet.size = Vector2(window.TextureSize[tileSet.tex].x, window.TextureSize[tileSet.tex].y);

	topBar.size = Vector2(window.getSize().x, 48);
	topBar.texturePos.x = 123;
	topBar.texturePos.y = 2;
	topBar.texturePos.w = 44;
	topBar.texturePos.h = 11;

	sideBar.size = Vector2(tileSet.size.x + TILE_SIZE, window.getSize().y);
	sideBar.texturePos.x = 123;
	sideBar.texturePos.y = 2;
	sideBar.texturePos.w = 44;
	sideBar.texturePos.h = 11;

	tileSet.transform = Vector2(0, TILE_SIZE * 2);
	tileSet.texturePos.h = window.TextureSize[tileSet.tex].y;
	tileSet.texturePos.w = window.TextureSize[tileSet.tex].x;

	if (layer1.onClick())
	{
		activeLayer = 1;

		for (unsigned int i = 0; i < level.tiles.size(); i++)
		{
			if (level.tiles[i].layer != activeLayer)
			{
				level.tiles[i].color.a = 0.5;
			}
			else
			{
				level.tiles[i].color.a = 1;
			}
		}
	}

	if (layer2.onClick())
	{
		activeLayer = 2;

		for (unsigned int i = 0; i < level.tiles.size(); i++)
		{
			if (level.tiles[i].layer != activeLayer)
			{
				level.tiles[i].color.a = 0.5;
			}
			else
			{
				level.tiles[i].color.a = 1;
			}
		}
	}

	if (layer3.onClick())
	{
		activeLayer = 3;

		for (unsigned int i = 0; i < level.tiles.size(); i++)
		{
			if (level.tiles[i].layer != activeLayer)
			{
				level.tiles[i].color.a = 0.5;
			}
			else
			{
				level.tiles[i].color.a = 1;
			}
		}
	}

	if (layer4.onClick())
	{
		activeLayer = 4;

		for (unsigned int i = 0; i < level.tiles.size(); i++)
		{
			if (level.tiles[i].layer != activeLayer)
			{
				level.tiles[i].color.a = 0.5;
			}
			else
			{
				level.tiles[i].color.a = 1;
			}
		}
	}

	switch (activeLayer)
	{
	case 1:
		layer1.color.r = 0.5;
		layer1.color.g = 0.5;
		layer1.color.b = 0.5;

		layer2.color.r = 1;
		layer2.color.g = 1;
		layer2.color.b = 1;

		layer3.color.r = 1;
		layer3.color.g = 1;
		layer3.color.b = 1;

		layer4.color.r = 1;
		layer4.color.g = 1;
		layer4.color.b = 1;
		break;
	case 2:
		layer1.color.r = 1;
		layer1.color.g = 1;
		layer1.color.b = 1;

		layer2.color.r = 0.5;
		layer2.color.g = 0.5;
		layer2.color.b = 0.5;

		layer3.color.r = 1;
		layer3.color.g = 1;
		layer3.color.b = 1;

		layer4.color.r = 1;
		layer4.color.g = 1;
		layer4.color.b = 1;
		break;
	case 3:
		layer1.color.r = 1;
		layer1.color.g = 1;
		layer1.color.b = 1;

		layer2.color.r = 1;
		layer2.color.g = 1;
		layer2.color.b = 1;

		layer3.color.r = 0.5;
		layer3.color.g = 0.5;
		layer3.color.b = 0.5;

		layer4.color.r = 1;
		layer4.color.g = 1;
		layer4.color.b = 1;
		break;
	case 4:
		layer1.color.r = 1;
		layer1.color.g = 1;
		layer1.color.b = 1;

		layer2.color.r = 1;
		layer2.color.g = 1;
		layer2.color.b = 1;

		layer3.color.r = 1;
		layer3.color.g = 1;
		layer3.color.b = 1;

		layer4.color.r = 0.5;
		layer4.color.g = 0.5;
		layer4.color.b = 0.5;
		break;
	}

	if (collisionButton.onClick())
	{
		if (viewCollision == false)
		{
			viewCollision = true;
		}
		else
		{
			viewCollision = false;
		}
	}

	collisionView.clear();

	if (viewCollision == true)
	{
		collisionButton.color.r = 0.5;
		collisionButton.color.g = 0.5;
		collisionButton.color.b = 0.5;
		editingPlayer = false;
		editingLights = false;

		Entity collisionTile(Vector2(TILE_SIZE, TILE_SIZE));
		collisionTile.tex = collisionTexture;
		collisionTile.layer = 5;

		for (const Entity& wall : level.tiles)
		{
			if (wall.colUp == true || wall.colDown == true || wall.colLeft == true || wall.colRight == true)
			{
				if (wall.layer == activeLayer)
				{
					collisionTile.transform = wall.transform;
					collisionView.push_back(collisionTile);
				}
			}
		}
	}
	else
	{
		collisionButton.color.r = 1;
		collisionButton.color.g = 1;
		collisionButton.color.b = 1;
	}

	if (playerButton.onClick())
	{
		if (editingPlayer == false)
		{
			editingPlayer = true;
		}
		else
		{
			editingPlayer = false;
		}
	}

	if (editingPlayer == true)
	{
		playerButton.color.r = 0.5;
		playerButton.color.g = 0.5;
		playerButton.color.b = 0.5;
		viewCollision = false;
		editingLights = false;
	}
	else
	{
		playerButton.color.r = 1;
		playerButton.color.g = 1;
		playerButton.color.b = 1;
	}

	if (lightsButton.onClick())
	{
		if (editingLights == false)
		{
			editingLights = true;
		}
		else
		{
			editingLights = false;
		}
	}

	if (editingLights == true)
	{
		lightsButton.color.r = 0.5;
		lightsButton.color.g = 0.5;
		lightsButton.color.b = 0.5;
		viewCollision = false;
		editingPlayer = false;

		lightCursor.intensity = brightness.value;
		lightCursor.radius = radius.value * 1000;
		lightCursor.r = red.value;
		lightCursor.g = green.value;
		lightCursor.b = blue.value;
	}
	else
	{
		lightsButton.color.r = 1;
		lightsButton.color.g = 1;
		lightsButton.color.b = 1;
	}

	if (tileSet.onClick())
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		x = (x / TILE_SIZE) * TILE_SIZE;
		y = (y / TILE_SIZE) * TILE_SIZE;
		selector.transform = Vector2(x, y);

		if (selector.transform.x >= tileSet.size.x - TILE_SIZE)
		{
			selector.transform.x = tileSet.size.x - TILE_SIZE;
		}
	}

	if (!topBar.touchingMouse() && !sideBar.touchingMouse())
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		int x2 = x, y2 = y;

		if (((x + (window.cameraPos.x)) - window.getSize().x / 2) <= 0)
		{
			x = x - TILE_SIZE; // this needs to change if we use anything other than 24 x 24 tiles
		}

		if (((y + (window.cameraPos.y)) - window.getSize().y / 2) <= 0)
		{
			y = y - TILE_SIZE;
		}

		x = (x + (window.cameraPos.x) - window.getSize().x / 2);
		y = (y + (window.cameraPos.y) - window.getSize().y / 2);

		x2 = (x2 + (window.cameraPos.x) - window.getSize().x / 2);
		y2 = (y2 + (window.cameraPos.y) - window.getSize().y / 2);

		x = (x / TILE_SIZE) * TILE_SIZE;
		y = (y / TILE_SIZE) * TILE_SIZE;

		if (viewCollision == true)
		{
			cursor.color.a = 1;
			cursor.tex = collisionTexture;
			cursor.texturePos.x = 0;
			cursor.texturePos.y = 0;
			cursor.transform = Vector2(x, y);
		}
		else if (editingPlayer == true)
		{
			cursor.color.a = 0.5;
			cursor.tex = playerSpawn.tex;
			cursor.texturePos.x = 0;
			cursor.texturePos.y = 0;
			cursor.transform = Vector2(x, y);
		}
		else if (editingLights == true)
		{
			lightCursor.layer = activeLayer;
			lightCursor.transform = Vector2(x2, y2);
		}
		else
		{
			cursor.color.a = 0.5;
			cursor.tex = tileSet.tex;
			cursor.texturePos.x = selector.transform.x;
			cursor.texturePos.y = -(selector.transform.y - (tileSet.size.y + TILE_SIZE));
			cursor.transform = Vector2(x, y);
		}

		if (Event::MousePressed(SDL_BUTTON_RIGHT) && viewCollision == false && editingPlayer == false && editingLights == false)
		{
			for (unsigned int i = 0; i < level.tiles.size(); i++)
			{
				if (level.tiles[i].transform.x == x && level.tiles[i].transform.y == y && level.tiles[i].layer == activeLayer)
				{
					level.tiles.erase(level.tiles.begin() + i);
				}
			}
		}
		else if (Event::MousePressed(SDL_BUTTON_RIGHT) && viewCollision == true)
		{
			for (unsigned int i = 0; i < level.tiles.size(); i++)
			{
				if (level.tiles[i].transform.x == x && level.tiles[i].transform.y == y && level.tiles[i].layer == activeLayer)
				{
					level.tiles[i].colUp = false;
					level.tiles[i].colDown = false;
					level.tiles[i].colLeft = false;
					level.tiles[i].colRight = false;
				}
			}
		}
		else if (Event::MousePressed(SDL_BUTTON_RIGHT) && editingLights == true)
		{
			for (unsigned int i = 0; i < level.lights.size(); i++)
			{
				if ((Vector2(level.lights[i].transform.x - x2, level.lights[i].transform.y - y2).magnitude()) < 30) // this is stupid as fuck but it should probably work?
				{
					level.lights.erase(level.lights.begin() + i);
				}
			}
		}

		if (Event::MousePressed(SDL_BUTTON_LEFT) && viewCollision == false && editingPlayer == false && editingLights == false)
		{
			Entity tile(Vector2(TILE_SIZE, TILE_SIZE));
			tile.tex = tileSet.tex;
			tile.texturePos.x = selector.transform.x;
			tile.texturePos.y = -(selector.transform.y - (tileSet.size.y + TILE_SIZE));
			tile.layer = activeLayer;
			tile.colUp = false;
			tile.colDown = false;
			tile.colLeft = false;
			tile.colRight = false;

			tile.transform = Vector2(x, y);

			bool obstructed = false;

			for (const Entity& wall : level.tiles)
			{
				if (wall.transform.x == x && wall.transform.y == y && wall.layer == tile.layer)
				{
					obstructed = true;
				}
			}
			if (obstructed == false)
			{
				level.tiles.push_back(tile);
			}
		}
		else if (Event::MousePressed(SDL_BUTTON_LEFT) && viewCollision == true)
		{
			for (unsigned int i = 0; i < level.tiles.size(); i++)
			{
				if (level.tiles[i].transform.x == x && level.tiles[i].transform.y == y && level.tiles[i].layer == activeLayer)
				{
					level.tiles[i].colUp = true;
					level.tiles[i].colDown = true;
					level.tiles[i].colLeft = true;
					level.tiles[i].colRight = true;
				}
			}
		}
		else if (Event::MousePressed(SDL_BUTTON_LEFT) && editingPlayer == true)
		{
			playerSpawn.transform = Vector2(x, y);
		}
		else if (Event::MouseDown(SDL_BUTTON_LEFT) && editingLights == true)
		{
			level.lights.push_back(lightCursor);
		}
	}

	if (Event::MousePressed(SDL_BUTTON_MIDDLE))
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		//x = x / window.zoomX;
		//y = y / window.zoomY;
		window.camera(Vector2(-x - offsetMouse.x + offsetCam.x, -y - offsetMouse.y + offsetCam.y));
	}
	else if (!Event::MousePressed(SDL_BUTTON_MIDDLE))
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		//x = x / window.zoomX;
		//y = y / window.zoomY;

		offsetMouse = Vector2(-x, -y);
		offsetCam = window.cameraPos;
	}

	if (saveButton.onClick())
	{
		if (currentFile.length() == 0)
		{
			char filename[MAX_PATH];

			OPENFILENAME ofn;
			ZeroMemory(&filename, sizeof(filename));
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
			ofn.lpstrFilter = "Moss Engine Level Files (*.lvl)\0*.lvl\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrTitle = "Save";
			ofn.lpstrDefExt = "*.lvl\0";
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;

			if (GetSaveFileNameA(&ofn))
			{
				std::cout << "You chose the file \"" << filename << "\"\n";
			}

			std::cout << std::endl << "Saving..." << std::endl;

			if (std::string(filename).length() > 0)
			{
				Level::SaveLevel(std::string(filename), level);
				currentFile = std::string(filename);

				std::cout << std::endl << "Saving complete!" << std::endl;
			}
			else
			{
				std::cout << std::endl << "Saving Failed! Did you choose a file?" << std::endl;
			}
		}
		else
		{
			std::cout << std::endl << "Saving..." << std::endl;

			Level::SaveLevel(currentFile, level);

			std::cout << std::endl << "Saving complete!" << std::endl;
		}
	}


	if (loadButton.onClick())
	{

		char filename[MAX_PATH];

		OPENFILENAME ofn;
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
		ofn.lpstrFilter = "Moss Engine Level Files (*.lvl)\0*.lvl\0Any File\0*.*\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Open";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn))
		{
			std::cout << "You chose the file \"" << filename << "\"\n";
		}

		std::cout << std::endl << "Loading..." << std::endl;


		if (std::string(filename).length() > 0)
		{
			//std::string activeTexture = Level::LoadFile(std::string(filename))[0];

			//loadTextures(activeTexture);
			//Level::LoadLevel(Level::LoadFile(std::string(filename)), walls, window);
			level = Level::Load(std::string(filename));

			currentFile = std::string(filename);

			std::cout << std::endl << "Loading complete!" << std::endl;
		}
		else
		{
			std::cout << std::endl << "Loading Failed! Did you choose a file?" << std::endl;
		}
	}

	if (importButton.onClick())
	{
		char filename[MAX_PATH];

		OPENFILENAME ofn;
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
		ofn.lpstrFilter = "PNG (*.png)\0*.png\0Any File\0*.*\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Open";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		//PathStripPath(ofn.lpstrFile);

		if (std::string(filename).length() > 0)
		{
			activeTexture = "assets/textures/" + std::string(ofn.lpstrFile);

			loadTextures(activeTexture);
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
}

void render() // honestly i feel like putting the stuff that is at the end of the gameloop in here
{
  	// for (std::map<int,Entity>::iterator it = LivingEnts.begin(); it != LivingEnts.end(); ++it)
  	// {
  	// 	window.render(it->second, true);
  	// }

	for (Light light : level.lights)
	{
		window.render(light);
	}

	for (Entity wall : level.tiles)
	{
		window.render(wall, true);
	}

	for (Entity collision : collisionView)
	{
		window.render(collision, true);
	}

	if (editingPlayer == true)
	{
		window.render(playerSpawn, true);
	}

	if (editingLights)
	{
		window.render(lightCursor);
	}
	else
	{
		window.render(cursor, true);
	}

	window.render(tileSet);

	window.render(sideBar);
	window.render(topBar);

	window.render(selector, false);

	window.render(layer1);
	window.render(layer2);
	window.render(layer3);
	window.render(layer4);

	window.render(collisionButton);

	window.render(lightsButton);

	window.render(saveButton);

	window.render(saveAsButton);

	window.render(loadButton);

	window.render(playerButton);

	window.render(importButton);

	window.render(brightness);
	window.render(radius);
	window.render(red);
	window.render(green);
	window.render(blue);
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
	
	window.quit(); // run when user asks to exit program
	Mix_Quit();
	SDL_Quit();
	SteamAPI_Shutdown();
	return 0;
}