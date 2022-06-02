#pragma once

#include <string>
#include <vector>

#include "entity.hpp"
#include "renderwindow.hpp"
#include "math.hpp"

namespace Level
{
	std::vector<std::string> LoadFile(std::string path);
	Vector2 LoadLevel(std::vector<std::string> data, std::vector<Entity>& p_ent, RenderWindow& p_win);
	void SaveLevel(std::string path, std::vector<Entity> &tiles, std::string texPath);
};
