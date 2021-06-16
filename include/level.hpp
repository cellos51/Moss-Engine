#pragma once

#include <string>
#include <vector>

#include "entity.hpp"

#include "renderwindow.hpp"

namespace Level
{
	std::vector<std::string> LoadFile(std::string path);
	void LoadLevel(std::vector<std::string> data, std::vector<Entity>& p_ent, RenderWindow& p_win, SDL_Texture* p_tex[]);
	void SaveLevel(std::string path, std::vector<std::string> lvl);
}