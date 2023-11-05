#include "gamescene.hpp"

GameScene::GameScene(OpenGLWindow& window) : window(window)
{

}

void GameScene::onStart()
{
	window.loadTexture("assets/textures/tile_01.png");
	level = level = Level::Load("assets/levels/level.lvl");

	playerLight.layer = 2;
	playerLight.intensity = 0.1f;
	playerLight.g = 0.8f;
	playerLight.b = 0.8f;

	player.tex = window.loadTexture("assets/textures/player.png");
	player.transform = level.spawn;
	player.luminosity = Color4(0.2f, 0.2f, 0.2f, 0.0f);
	player.layer = 2;
	player.texturePos.w = 64;
	player.texturePos.h = 64;
	player.offset.w = 64;
	player.offset.h = 64;

	player.offset.x = -player.size.x / 2;
	player.offset.y = -32;

	window.camera(Vector2(player.transform.x + player.size.x / 2, player.transform.y + player.size.y / 2));
	window.setZoom(3);
	window.clampAmount = 100;
	window.lerpAmount = 0.005;
}

void GameScene::onEnd()
{

}

void GameScene::update()
{
	player.update();

	for (auto& ent : level.tiles)
	{
		player.getCol(ent);
	}

	playerLight.transform = Vector2(player.transform.x + player.size.x / 2, player.transform.y + player.size.y / 2);

	window.camera(Vector2(player.transform.x + player.size.x / 2, (player.transform.y + player.size.y / 2) - 32));
}
void GameScene::fixedUpdate()
{

}

void GameScene::render(OpenGLWindow& window)
{
	for (Light light : level.lights)
	{
		window.render(light);
	}

	for (Entity wall : level.tiles)
	{
		window.render(wall, true);
	}

	window.render(player, true);
	window.render(playerLight);
}