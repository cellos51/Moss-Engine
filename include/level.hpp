#pragma once

#include <string>
#include <vector>

#include "entity.hpp"
#include "light.hpp"
#include "openglwindow.hpp"
#include "math.hpp"

struct LevelData
{
	Vector2 spawn = Vector2(0.0f, 0.0f);
	std::vector<Entity> tiles;
	std::vector<Light> lights;
};

namespace Level
{
	LevelData Load(std::string path);
	void SaveLevel(std::string path, LevelData& data);
}