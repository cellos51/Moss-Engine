#include "level.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "entity.hpp"

#include "renderwindow.hpp"

void checkAdjacent(Entity& _ent, int _a, int _b, std::vector<std::string> lvl)
{
	if (_b + _a * 16 + 2 - 16 >= 0)
	{
		if (lvl[_b + _a * 16 + 2 - 16] != "0")
		{
			_ent.colUp = false;
		}

		if (lvl[_b + _a * 16 + 2 - 1] != "0")
		{
			_ent.colLeft = false;
		}
	}

	if (_b + _a * 16 + 2 + 16 <= std::stoi(lvl[0]) * std::stoi(lvl[1]))
	{
		if (lvl[_b + _a * 16 + 2 + 16] != "0")
		{
			_ent.colDown = false;
		}

		if (lvl[_b + _a * 16 + 2 + 1] != "0")
		{
			_ent.colRight = false;
		}
	}
}

void Level::LoadLevel(std::string path, std::vector<Entity>& p_ent, RenderWindow& p_win)
{	
	SDL_Texture* grass = p_win.loadTexture("assets/textures/grass.png");
	SDL_Texture* dirt1 = p_win.loadTexture("assets/textures/dirt1.png");
	SDL_Texture* dirt2 = p_win.loadTexture("assets/textures/dirt2.png");
	SDL_Texture* dirt3 = p_win.loadTexture("assets/textures/dirt3.png");
	SDL_Texture* dirt4 = p_win.loadTexture("assets/textures/dirt4.png");
	SDL_Texture* dirt5 = p_win.loadTexture("assets/textures/dirt5.png");
	SDL_Texture* dirt6 = p_win.loadTexture("assets/textures/dirt6.png");
	SDL_Texture* dirt7 = p_win.loadTexture("assets/textures/dirt7.png");
	SDL_Texture* dirt8 = p_win.loadTexture("assets/textures/dirt8.png"); 
	SDL_Texture* dirt9 = p_win.loadTexture("assets/textures/dirt9.png");
	SDL_Texture* dirt10 = p_win.loadTexture("assets/textures/dirt10.png");
	SDL_Texture* dirt11 = p_win.loadTexture("assets/textures/dirt11.png");
	SDL_Texture* dirt12 = p_win.loadTexture("assets/textures/dirt12.png");
	SDL_Texture* dirt13 = p_win.loadTexture("assets/textures/dirt13.png");


	std::ifstream inFile(path); // file based level loader (will be moved to its own class)
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


	int a, b;
	for (a = 0; a < std::stoi(level[0]); a++) // make these numbers the same as "level"
	{
		for (b = 0; b < std::stoi(level[1]); b++)
		{
			switch (std::stoi(level[b + a * 16 + 2]))
			{
				case 1:
				{
					Entity ent (Vector2(b * 64, a * 64), grass, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 2:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt1, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 3:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt2, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 4:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt3, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 5:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt4, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 6:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt5, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 7:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt6, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 8:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt7, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 9:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt8, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 10:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt9, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 11:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt10, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 12:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt11, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 13:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt12, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;

				case 14:
				{
					Entity ent (Vector2(b * 64, a * 64), dirt13, Vector2(64,64));
					checkAdjacent(ent, a, b, level);
					p_ent.push_back (ent);
				}
				break;
			}
		}
	}
}