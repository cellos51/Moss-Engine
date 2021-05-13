#pragma once

#include <string>
#include <vector>

#include "entity.hpp"

#include "renderwindow.hpp"

namespace Level
{
	void LoadLevel(std::string path, std::vector<Entity>& p_ent, RenderWindow& p_win);
}