#include "level.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "entity.hpp"
#include "math.hpp"
#include "renderwindow.hpp"

const int tileSize = 24;

// void checkAdjacent(Entity& _ent, int _a, int _b, std::vector<std::string> lvl)
// {
// 	if (_b + _a * std::stoi(lvl[1]) + 2 - std::stoi(lvl[1]) >= 0)
// 	{
// 		if (lvl[_b + _a * std::stoi(lvl[1]) + 2 - std::stoi(lvl[1])] != "0" && lvl[_b + _a * std::stoi(lvl[1]) + 2 - std::stoi(lvl[1])] != "15")
// 		{
// 			_ent.colUp = false;
// 		}

// 		if (lvl[_b + _a * std::stoi(lvl[1]) + 2 - 1] != "0" && lvl[_b + _a * std::stoi(lvl[1]) + 2 - 1] != "15")
// 		{
// 			_ent.colLeft = false;
// 		}
// 	}
// 	else
// 	{
// 		_ent.colUp = false;
// 		_ent.colLeft = false;
// 	}

// 	if (_b + _a * std::stoi(lvl[1]) + 2 + std::stoi(lvl[1]) <= std::stoi(lvl[0]) * std::stoi(lvl[1])) // todo fix bug where colright does not get set to false even if there is a tile next to it
// 	{
// 		if (lvl[_b + _a * std::stoi(lvl[1]) + 2 + std::stoi(lvl[1])] != "0" && lvl[_b + _a * std::stoi(lvl[1]) + 2 + std::stoi(lvl[1])] != "15")
// 		{
// 			_ent.colDown = false;
// 		}

// 		if (lvl[_b + _a * std::stoi(lvl[1]) + 2 + 1] != "0" && lvl[_b + _a * std::stoi(lvl[1]) + 2 + 1] != "15")
// 		{
// 			_ent.colRight = false;
// 		}
// 	}
// 	else
// 	{
// 		_ent.colDown = false;
// 		_ent.colRight = false;
// 	}
// }

//std::string texturePath;

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
                std::cout << course << std::endl;
            }
        }
    }
    inFile.close();

    //texturePath = level[0];

    return level;
}

Vector2 Level::LoadLevel(std::vector<std::string> data, std::vector<Entity>& p_ent, RenderWindow& p_win)
{	
	Vector2 pspawn = Vector2(0,0);

	unsigned int texture = p_win.loadTexture(data[0].c_str());
	
	p_ent.clear();
	p_ent.shrink_to_fit();

	//std::vector<std::string> level;

    //std::string line;

    for (unsigned int i = 1; i < data.size(); i++)
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

        // ent.texturePos.x = 24;
        // ent.texturePos.y = 312;
        // ent.texturePos.w = 24;
        // ent.texturePos.h = 24;

        ent.colUp = tileParamters[7];
        ent.colDown = tileParamters[8];
        ent.colLeft = tileParamters[9];
        ent.colRight = tileParamters[10];

		p_ent.push_back (ent);
	}



	// int a, b; // these comments are fucking pointless now. on another note listen to lemon demon and red vox fucking amazing bands.
	// for (a = 0; a < std::stoi(data[0]); a++) // make these numbers the same as "level"
	// {
	// 	for (b = 0; b < std::stoi(data[1]); b++)
	// 	{
	// 		switch (std::stoi(data[b + a * std::stoi(data[1]) + 2]))
	// 		{
	// 			case 1:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 2:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 3:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 4:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 5:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 6:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 7:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 8:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 9:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 10:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 11:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 12:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 13:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 14:
	// 			{
	// 				Entity ent (Vector2(b * tileSize, a * tileSize), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(tileSize,tileSize));
	// 				checkAdjacent(ent, a, b, data);
	// 				p_ent.push_back (ent);
	// 			}
	// 			break;

	// 			case 15:
	// 			{
	// 				pspawn = Vector2(b * tileSize, a * tileSize);
	// 			}
	// 			break;
	// 		}
	// 	}
	// }
	return pspawn;
} // woulnd't it be funny if i just started venting my problems into this as comments and have a diary inside this? i might actually do that

void Level::SaveLevel(std::string path, std::vector<Entity> &tiles, std::string texPath)
{	
	std::string levelData;
	levelData = levelData + texPath + "\n";

	for (Entity wall : tiles)
	{
		wall.colUp = true;
		wall.colDown = true;
		wall.colLeft = true;
		wall.colRight = true;

		for (Entity wallz : tiles)
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