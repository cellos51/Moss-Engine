#pragma once

#include "scene.hpp"

#include "openglwindow.hpp"
#include "entity.hpp"
#include "math.hpp"
#include "level.hpp"
#include "player.hpp"
#include "unordered_map"

class SplashScene : public Scene
{
public:
	SplashScene(OpenGLWindow& window);
	void onStart() override;
	void onEnd() override;
	void update() override;
	void fixedUpdate() override;
	void render(OpenGLWindow& window) override;
private:
	OpenGLWindow& window;
	
	ui::Panel splash;
	ui::Panel background;
	float timer = 0.0f;
};

