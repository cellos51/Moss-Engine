#pragma once

#include "scene.hpp"

#include "openglwindow.hpp"
#include "entity.hpp"
#include "math.hpp"
#include "level.hpp"
#include "player.hpp"
#include "unordered_map"

class GameScene : public Scene
{
public:
	GameScene(OpenGLWindow& window);
	void onStart() override;
	void onEnd() override;
	void update() override;
	void fixedUpdate(double deltaTime) override;
	void render(OpenGLWindow& window) override;
private:
	OpenGLWindow& window;

	LevelData level;

	std::vector<Entity> tiles;
	std::vector<Entity> collidableTiles;

	Player player = Player();
	Light playerLight;

	std::unordered_map<unsigned long int, NetPlayer> networkPlayers;
};

