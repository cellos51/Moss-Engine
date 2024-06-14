#include "level.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "entity.hpp"
#include "math.hpp"
#include "openglwindow.hpp"

Vector2 playerSpawnPoint = Vector2(0,0);

std::string texturePath;

LevelData Level::Load(std::string path) // new shit incoming
{	
	LevelData data;

	std::ifstream inFile(path);
	if (inFile.is_open())
	{
		// Spawn Point
		inFile.read((char*)&data.spawn, sizeof(Vector2));

		// Ambient Light
		inFile.read((char*)&data.ambientLight, sizeof(Color4));

		// Tiles
		typename std::vector<Entity>::size_type tilesSize = 0;
		inFile.read((char*)&tilesSize, sizeof(tilesSize));
		data.tiles.resize(tilesSize);
		inFile.read((char*)&data.tiles[0], data.tiles.size() * sizeof(Entity));

		// Lights
		typename std::vector<Light>::size_type lightSize = 0;
		inFile.read((char*)&lightSize, sizeof(lightSize));
		data.lights.resize(lightSize);
		inFile.read((char*)&data.lights[0], data.lights.size() * sizeof(Light));
	}
	inFile.close();

	return data;
}

void Level::SaveLevel(std::string path, LevelData& data)
{	
	std::vector<Entity> optimizedTiles;

	for (Entity tile1 : data.tiles)
	{
		for (const Entity& tile2 : data.tiles)
		{
			if (tile2.colUp == true || tile2.colDown == true || tile2.colLeft == true || tile2.colRight == true)
			{
				if ((tile1.transform.y + tile1.size.y) == tile2.transform.y && tile1.transform.x == tile2.transform.x)
				{
					tile1.colDown = false;
				}

				if ((tile1.transform.y - tile1.size.y) == tile2.transform.y && tile1.transform.x == tile2.transform.x)
				{
					tile1.colUp = false;
				}

				if ((tile1.transform.x + tile1.size.x) == tile2.transform.x && tile1.transform.y == tile2.transform.y)
				{
					tile1.colRight = false;
				}

				if ((tile1.transform.x - tile1.size.x) == tile2.transform.x && tile1.transform.y == tile2.transform.y)
				{
					tile1.colLeft = false;
				}
			}
		}

		optimizedTiles.push_back(tile1);
	}

	std::ofstream outFile(path);
    if (outFile.is_open())
  	{
		// Spawn Point
		outFile.write((char*)&data.spawn, sizeof(Vector2));

		// Ambient Light
		outFile.write((char*)&data.ambientLight, sizeof(Color4));

		// Tiles
		std::vector<Entity>::size_type entSize = optimizedTiles.size();
		outFile.write((char*)&entSize, sizeof(entSize));
		outFile.write((char*)&optimizedTiles[0], optimizedTiles.size() * sizeof(Entity));

		// Lights
		std::vector<Light>::size_type lightSize = data.lights.size();
		outFile.write((char*)&lightSize, sizeof(lightSize));
		outFile.write((char*)&data.lights[0], data.lights.size() * sizeof(Light));
    }
    outFile.close();
}