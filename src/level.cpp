#include "level.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "entity.hpp"

#include "math.hpp"

#include "renderwindow.hpp"

void checkAdjacent(Entity& _ent, int _a, int _b, std::vector<std::string> lvl)
{
	if (_b + _a * std::stoi(lvl[1]) + 2 - std::stoi(lvl[1]) >= 0)
	{
		if (lvl[_b + _a * std::stoi(lvl[1]) + 2 - std::stoi(lvl[1])] != "0" && lvl[_b + _a * std::stoi(lvl[1]) + 2 - std::stoi(lvl[1])] != "15")
		{
			_ent.colUp = false;
		}

		if (lvl[_b + _a * std::stoi(lvl[1]) + 2 - 1] != "0" && lvl[_b + _a * std::stoi(lvl[1]) + 2 - 1] != "15")
		{
			_ent.colLeft = false;
		}
	}
	else
	{
		_ent.colUp = false;
		_ent.colLeft = false;
	}

	if (_b + _a * std::stoi(lvl[1]) + 2 + std::stoi(lvl[1]) <= std::stoi(lvl[0]) * std::stoi(lvl[1])) // todo fix bug where colright does not get set to false even if there is a tile next to it
	{
		if (lvl[_b + _a * std::stoi(lvl[1]) + 2 + std::stoi(lvl[1])] != "0" && lvl[_b + _a * std::stoi(lvl[1]) + 2 + std::stoi(lvl[1])] != "15")
		{
			_ent.colDown = false;
		}

		if (lvl[_b + _a * std::stoi(lvl[1]) + 2 + 1] != "0" && lvl[_b + _a * std::stoi(lvl[1]) + 2 + 1] != "15")
		{
			_ent.colRight = false;
		}
	}
	else
	{
		_ent.colDown = false;
		_ent.colRight = false;
	}
}

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
            while (std::getline(ss,course,','))
            {
                level.push_back(course);
            }
        }
    }
    inFile.close();

    return level;
}

Vector2 Level::LoadLevel(std::vector<std::string> data, std::vector<Entity>& p_ent, RenderWindow& p_win, unsigned int p_tex[])
{	
	Vector2 pspawn = Vector2(0,0);

	p_ent.clear();
	p_ent.shrink_to_fit();

	int a, b; // these comments are fucking pointless now. on another note listen to lemon demon and red vox fucking amazing bands.
	for (a = 0; a < std::stoi(data[0]); a++) // make these numbers the same as "level"
	{
		for (b = 0; b < std::stoi(data[1]); b++)
		{
			switch (std::stoi(data[b + a * std::stoi(data[1]) + 2]))
			{
				case 1:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 2:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 3:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 4:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 5:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 6:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 7:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 8:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 9:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 10:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 11:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 12:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 13:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 14:
				{
					Entity ent (Vector2(b * 16, a * 16), p_tex[std::stoi(data[b + a * std::stoi(data[1]) + 2]) - 1], Vector2(16,16));
					checkAdjacent(ent, a, b, data);
					p_ent.push_back (ent);
				}
				break;

				case 15:
				{
					pspawn = Vector2(b * 16, a * 16);
				}
				break;
			}
		}
	}
	return pspawn;
} // woulnd't it be funny if i just started venting my problems into this as comments and have a diary inside this? i might actually do that

void Level::SaveLevel(std::string path, std::vector<std::string> lvl)
{	
	std::ofstream outFile(path);
    if (outFile.is_open())
  	{
    	for(unsigned i = 0; i < lvl.size(); i ++)
    	{
        outFile << lvl[i] << "," ;
    	}
    }
    outFile.close();
}