#include "gamescene.hpp"

#include "console.hpp"
#include "event.hpp"
#include "networking.hpp"


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

	networkTest.tex = player.tex;
	networkTest.offset = player.offset;
	networkTest.size = player.size;
	networkTest.texturePos = player.texturePos;
	networkTest.transform = level.spawn;
	networkTest.layer = player.layer;
	networkTest.luminosity = player.luminosity;


	window.camera(Vector2(player.transform.x + player.size.x / 2, player.transform.y + player.size.y / 2));
	window.setZoom(3);
	window.clampAmount = Vector2(10,50);
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
	if (console.enabled == false)
	{
		if (Event::KeyPressed(SDLK_1))
		{
			//window.zoom = 0;
			window.setZoom(1);
		}
		else if (Event::KeyPressed(SDLK_2))
		{
			//window.zoom = 0.25;
			window.setZoom(2);
		}
		else if (Event::KeyPressed(SDLK_3))
		{
			//window.zoom = 0.5;
			window.setZoom(3);
		}
		else if (Event::KeyPressed(SDLK_4))
		{
			//window.zoom = 0.75;
			window.setZoom(4);
		}
	}
	uint32_t dataSize = sizeof(player.transform);
	std::unique_ptr<uint8_t[]> data(new uint8_t[dataSize]);

	memcpy(data.get(), &player.transform, dataSize);

	for (auto peer : steamSocket.peers)
	{
		steamSocket.sendMessage(peer, data.get(), dataSize, k_nSteamNetworkingSend_UnreliableNoDelay);
	}

	for (auto peer : steamSocket.peers)
	{
		SteamNetworkingMessage_t* messages[8];

		int receivedMessages = steamSocket.receiveMessages(peer, messages, 8);

		for (int i = 0; i < receivedMessages; i++)
		{
			const uint8_t* data = static_cast<const uint8_t*>(messages[i]->GetData());
			uint32_t dataSize = messages[i]->GetSize();

			if (dataSize == sizeof(Vector2))
			{
				memcpy(&networkTest.transform, data, sizeof(Vector2));
			}

			messages[i]->Release();
		}
	}
	
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

	window.render(networkTest, true);
}