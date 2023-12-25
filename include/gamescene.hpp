#pragma once

#include "scene.hpp"

#include "openglwindow.hpp"
#include "entity.hpp"
#include "math.hpp"
#include "level.hpp"
#include "player.hpp"

class GameScene : public Scene
{
public:
	GameScene(OpenGLWindow& window);
	void onStart() override;
	void onEnd() override;
	void update() override;
	void fixedUpdate() override;
	void render(OpenGLWindow& window) override;
private:
	OpenGLWindow& window;

	LevelData level;
	Player player = Player(Vector2(24, 48));
	Light playerLight;

};

