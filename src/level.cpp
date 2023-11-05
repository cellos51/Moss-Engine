#include "level.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "global.hpp"
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
		typename std::vector<Entity>::size_type entSize = 0;
		inFile.read((char*)&entSize, sizeof(entSize));
		data.tiles.resize(entSize);
		inFile.read((char*)&data.tiles[0], data.tiles.size() * sizeof(Entity));

		typename std::vector<Light>::size_type lightSize = 0;
		inFile.read((char*)&lightSize, sizeof(lightSize));
		data.lights.resize(lightSize);
		inFile.read((char*)&data.lights[0], data.lights.size() * sizeof(Light));

		inFile.read((char*)&data.spawn, sizeof(Vector2));
	}
	inFile.close();

	return data;
}

void Level::SaveLevel(std::string path, LevelData& data)
{	
	std::vector<Entity> map;

	for (Entity wall : data.tiles)
	{
		for (const  Entity& wallz : data.tiles)
		{
			if (wallz.colUp == true || wallz.colDown == true || wallz.colLeft == true || wallz.colRight == true)
			{
				if ((wall.transform.y + wall.size.y) == wallz.transform.y && wall.transform.x == wallz.transform.x)
				{
					wall.colDown = false;
				}

				if ((wall.transform.y - wall.size.y) == wallz.transform.y && wall.transform.x == wallz.transform.x)
				{
					wall.colUp = false;
				}

				if ((wall.transform.x + wall.size.x) == wallz.transform.x && wall.transform.y == wallz.transform.y)
				{
					wall.colRight = false;
				}

				if ((wall.transform.x - wall.size.x) == wallz.transform.x && wall.transform.y == wallz.transform.y)
				{
					wall.colLeft = false;
				}
			}
		}

		map.push_back(wall);
	}

	std::ofstream outFile(path);
    if (outFile.is_open())
  	{
		typename std::vector<Entity>::size_type entSize = map.size();
		outFile.write((char*)&entSize, sizeof(entSize));
		outFile.write((char*)&map[0], map.size() * sizeof(Entity));

		typename std::vector<Light>::size_type lightSize = data.lights.size();
		outFile.write((char*)&lightSize, sizeof(lightSize));
		outFile.write((char*)&data.lights[0], data.lights.size() * sizeof(Light));

		outFile.write((char*)&data.spawn, sizeof(Vector2));

        //outFile << levelData;
    }
    outFile.close();
}