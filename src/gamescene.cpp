#include "gamescene.hpp"

#include "console.hpp"
#include "event.hpp"
#include "networking.hpp"
#include <ranges>


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

	// network shit
	Packet packet { steamSocket.netConnection, PLAYER_DATA };

	uint32_t dataSize = sizeof(Vector2) + sizeof(Packet);
	std::unique_ptr<uint8_t[]> data(new uint8_t[dataSize]);

	memcpy(data.get(), &packet, sizeof(Packet)); // copy packet data into message
	memcpy(data.get() + sizeof(Packet), &player.transform, sizeof(Vector2)); // copy player data into message

	for (auto peer : steamSocket.peers) // sending messages
	{
		steamSocket.sendMessage(peer, data.get(), dataSize, k_nSteamNetworkingSend_UnreliableNoDelay);
	}

	for (auto peer : steamSocket.peers) // receiving messages
	{
		const int maxMessages = 8;

		SteamNetworkingMessage_t* messages[maxMessages];
		int receivedMessages = steamSocket.receiveMessages(peer, messages, maxMessages);

		for (int i = 0; i < receivedMessages; i++)
		{
			const uint8_t* receivedData = static_cast<const uint8_t*>(messages[i]->GetData());
			uint32_t receivedDataSize = messages[i]->GetSize();

			Packet receivedPacket{};

			memcpy(&receivedPacket, receivedData, sizeof(Packet));

			if (networkPlayers.find(receivedPacket.peerID) == networkPlayers.end()) // add new player to our list if we don't know who it's from yet (id 0 is always from server)
			{
				Entity networkPlayer;

				networkPlayer.tex = player.tex;
				networkPlayer.offset = player.offset;
				networkPlayer.size = player.size;
				networkPlayer.texturePos = player.texturePos;
				networkPlayer.transform = level.spawn;
				networkPlayer.layer = player.layer;
				networkPlayer.luminosity = player.luminosity;

				networkPlayers.insert(std::pair<unsigned long int, Entity>(receivedPacket.peerID, networkPlayer));
			}

			switch (receivedPacket.packetType)
			{
			case PLAYER_DATA:
				memcpy(&networkPlayers[receivedPacket.peerID].transform, receivedData + sizeof(Packet), sizeof(Vector2));
				break;
			}

			if (steamSocket.listenSocket > 0)
			{
				for (auto messagePeer : steamSocket.peers)
				{
					if (peer != messagePeer)
					{
						steamSocket.sendMessage(messagePeer, receivedData, receivedDataSize, k_nSteamNetworkingSend_UnreliableNoDelay);
					}
				}
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

	for (auto& [key, value] : networkPlayers)
	{
		window.render(value, true);
	}	
}