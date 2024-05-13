#include "splashscreenscene.hpp"
#include "gamescene.hpp"
#include "editorscene.hpp"

#include "console.hpp"
#include "event.hpp"
#include "networking.hpp"
#include <cmath>


SplashScene::SplashScene(OpenGLWindow& window) : window(window)
{
	
}

void SplashScene::onStart()
{
	splash.tex = 10;
	splash.size = Vector2(1100, 1100);
	splash.texturePos = SDL_Rect(0, 0, 1100, 1100);
	splash.color.a = 0;
	splash.layer = 10;

	background.tex = -1;
	background.color = Color4(0, 0, 0, 1);
	background.transform = Vector2(0, 0);
}

void SplashScene::onEnd()
{

}

void SplashScene::update()
{
	background.size = window.getSize();

	float size = (window.getSize().x < window.getSize().y) ? window.getSize().x : window.getSize().y;

	splash.size = Vector2(size, size);
	splash.transform = Vector2((window.getSize().x / 2) - (size / 2), (window.getSize().y / 2) - (size / 2));

	splash.color.a = sinf(timer);

	timer += Time::deltaTime() * 0.001;

	if (timer > M_PI)
	{
		activeScene.openScene(std::make_shared<GameScene>(window));
	}
}
void SplashScene::fixedUpdate(double deltaTime)
{

}

void SplashScene::render(OpenGLWindow& window)
{
	window.render(background);
	window.render(splash);
}