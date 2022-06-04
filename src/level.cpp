#include "level.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "entity.hpp"
#include "math.hpp"
#include "renderwindow.hpp"

const int tileSize = 24;

Vector2 playerSpawnPoint = Vector2(0,0);

std::string texturePath;

std::vector<std::string> Level::LoadFile(std::string path)
{

	std::ifstream inFile(path); // file based level loader (will be moved to its own class) hey future person here uhhh yeah i did that ok bye :)
	std::vector<std::string> level;
    if (inFile.is_open())
    {
        std::string line;
        while (std::getline(inFile,line))
        {
            std::stringstream ss(line);
            std::string course;
            while (std::getline(ss,course,'\n'))
            {
                level.push_back(course);
            }
        }
    }
    inFile.close();

    texturePath = level[0];

	std::stringstream ss(level[1]);
    std::string course;

    std::vector<std::string> tileParamters;

    while (std::getline(ss,course,','))
    {
        tileParamters.push_back(course);
    }

    playerSpawnPoint = Vector2(stoi(tileParamters[0]),stoi(tileParamters[1]));

    //std::cout << tileParamters[0] << " " << tileParamters[1];

    return level;
}

Vector2 Level::LoadLevel(std::vector<std::string> data, std::vector<Entity>& p_ent, RenderWindow& win)
{	

	unsigned int texture = win.loadTexture(texturePath.c_str());
	
	p_ent.clear();
	p_ent.shrink_to_fit();

	//std::vector<std::string> level;

    //std::string line;

	for (unsigned int i = 2; i < data.size(); i++)
	{
	    std::stringstream ss(data[i]);
	    std::string course;

	    std::vector<int> tileParamters;

	    while (std::getline(ss,course,','))
	    {
	        tileParamters.push_back(std::stoi(course));
	    }

		Entity ent (Vector2(tileParamters[0], tileParamters[1]), texture, Vector2(tileSize,tileSize));
	    ent.layer = tileParamters[2];
	    ent.texturePos.x = tileParamters[3];
	    ent.texturePos.y = tileParamters[4];
	    ent.texturePos.w = tileParamters[5];
	    ent.texturePos.h = tileParamters[6];


	    ent.colUp = tileParamters[7];
	    ent.colDown = tileParamters[8];
	    ent.colLeft = tileParamters[9];
	    ent.colRight = tileParamters[10];

		p_ent.push_back (ent);
	}

	return playerSpawnPoint;
} // woulnd't it be funny if i just started venting my problems into this as comments and have a diary inside this? i might actually do that

void Level::SaveLevel(std::string path, std::vector<Entity> &tiles, Entity spawnPoint, std::string texPath)
{	
	std::string levelData;
	levelData = levelData + texPath + "\n";
	levelData = levelData + std::to_string(int(spawnPoint.transform.x)) + "," + std::to_string(int(spawnPoint.transform.y)) + "\n";

	for (Entity wall : tiles)
	{
		for (Entity wallz : tiles)
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

		levelData = levelData + std::to_string(int(wall.transform.x)) + "," + std::to_string(int(wall.transform.y)) + ",";
		levelData = levelData + std::to_string(wall.layer) + ",";
		levelData = levelData + std::to_string(wall.texturePos.x) + "," + std::to_string(wall.texturePos.y) + "," + std::to_string(wall.texturePos.w) + "," + std::to_string(wall.texturePos.h) + ",";
		levelData = levelData +	std::to_string(wall.colUp) + "," +	std::to_string(wall.colDown) + "," +	std::to_string(wall.colLeft) + "," +	std::to_string(wall.colRight);
		levelData = levelData + "\n";
	}

	std::ofstream outFile(path);
    if (outFile.is_open())
  	{
        outFile << levelData;
    }
    outFile.close();
}