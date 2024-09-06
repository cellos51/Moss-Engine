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
	level = Level::Load("assets/levels/level.lvl");
	window.ambientLight = level.ambientLight;

	playerLight.layer = 6;
	playerLight.intensity = 0.1f;
	playerLight.g = 0.8f;
	playerLight.b = 0.8f;

	player.tex = 9;
	player.transform = level.spawn;

	window.camera(Vector2(player.transform.x + player.size.x / 2, player.transform.y + player.size.y / 2));
	window.setZoom(1);
	window.clampAmount = Vector2(10,50);
	window.lerpAmount = 0.005;

	for (Entity tile1 : level.tiles)
	{
		for (const Entity& tile2 : level.tiles)
		{
			if (tile2.colUp == true || tile2.colDown == true || tile2.colLeft == true || tile2.colRight == true)
			{
				if ((tile1.transform.y + tile1.size.y) == tile2.transform.y && tile1.transform.x == tile2.transform.x)
				{
					tile1.colDown = false;
				}

				if ((tile1.transform.y - tile1.size.y) == tile2.transform.y && tile1.transform.x == tile2.transform.x)
				{
					tile1.colUp = false;
				}

				if ((tile1.transform.x + tile1.size.x) == tile2.transform.x && tile1.transform.y == tile2.transform.y)
				{
					tile1.colRight = false;
				}

				if ((tile1.transform.x - tile1.size.x) == tile2.transform.x && tile1.transform.y == tile2.transform.y)
				{
					tile1.colLeft = false;
				}
			}
		}

		if (tile1.colDown || tile1.colUp || tile1.colLeft || tile1.colRight)
		{
			collidableTiles.push_back(tile1);
		}
		else
		{
			tiles.push_back(tile1);
		}
	}
}

void GameScene::onEnd()
{

}

void GameScene::fixedUpdate(double deltaTime)
{
	player.fixedUpdate(deltaTime);
	playerLight.transform = Vector2(player.transform.x + player.size.x / 2, player.transform.y + player.size.y / 2);
	window.grassDeform = Vector2(player.transform.x, player.transform.y + 4);

	for (auto& ent : collidableTiles)
	{
		player.getCol(ent, deltaTime);
	}

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

	if (steamSocket.peers.size() > 0) // only run this code if connected
	{
		if (steamSocket.listenSocket > 0 && networkPlayers.size() > steamSocket.peers.size()) // really bad way to handle disconnects but it doesn't matter that much since it doesn't happen often. this only runs on the server
		{
			HSteamNetConnection disconnectedPeer = 0;

			for (const auto& [key, value] : networkPlayers)
			{
				if (std::find(steamSocket.peers.begin(), steamSocket.peers.end(), key) == steamSocket.peers.end())
				{
					disconnectedPeer = key;
				}
			}

			if (disconnectedPeer != 0)
			{
				Packet packet{ steamSocket.netConnection, PLAYER_DISCONNECT };

				uint32_t dataSize = sizeof(PlayerData) + sizeof(Packet);
				std::unique_ptr<uint8_t[]> data(new uint8_t[dataSize]);

				memcpy(data.get(), &packet, sizeof(Packet)); // copy packet data into message
				memcpy(data.get() + sizeof(Packet), &disconnectedPeer, sizeof(HSteamNetConnection)); // copy player data into message

				for (auto peer : steamSocket.peers) // receiving messages
				{
					steamSocket.sendMessage(peer, data.get(), dataSize, k_nSteamNetworkingSend_Reliable);
				}

				networkPlayers.erase(disconnectedPeer);
			}
		}

		PlayerData playerData{ player.transform, player.velocity, player.OnGround, player.onWall() };

		Packet packet{ 0, PLAYER_DATA }; // all sent packets from the user go from 0. this probably isn't a good idea but idfk if it works it works.

		uint32_t dataSize = sizeof(PlayerData) + sizeof(Packet);
		std::unique_ptr<uint8_t[]> data(new uint8_t[dataSize]);

		memcpy(data.get(), &packet, sizeof(Packet)); // copy packet data into message
		memcpy(data.get() + sizeof(Packet), &playerData, sizeof(PlayerData)); // copy player data into message

		for (auto peer : steamSocket.peers) // receiving messages
		{
			steamSocket.sendMessage(peer, data.get(), dataSize, k_nSteamNetworkingSend_UnreliableNoDelay);

			const int maxMessages = 8;

			SteamNetworkingMessage_t* messages[maxMessages];
			int receivedMessages = steamSocket.receiveMessages(peer, messages, maxMessages);

			for (int i = 0; i < receivedMessages; i++)
			{
				if (steamSocket.listenSocket > 0) // if we're the server, manually set the data of the peer for future clients.
				{
					memcpy(messages[i]->m_pData, &peer, sizeof(HSteamNetConnection));
				}

				const uint8_t* receivedData = static_cast<const uint8_t*>(messages[i]->GetData());
				uint32_t receivedDataSize = messages[i]->GetSize();

				Packet receivedPacket{};

				memcpy(&receivedPacket, receivedData, sizeof(Packet));

				if (networkPlayers.find(receivedPacket.peerID) == networkPlayers.end()) // add new player to our list if we don't know who it's from yet (id 0 is always from server)
				{
					NetPlayer networkPlayer;

					networkPlayer.tex = player.tex;
					networkPlayer.transform = level.spawn;

					networkPlayers.insert(std::pair<unsigned long int, NetPlayer>(receivedPacket.peerID, networkPlayer));
				}

				switch (receivedPacket.packetType)
				{
					case PLAYER_DATA:
					{
						PlayerData receivedPlayerData;

						memcpy(&receivedPlayerData, receivedData + sizeof(Packet), sizeof(PlayerData));

						networkPlayers[receivedPacket.peerID].targetPos = receivedPlayerData.position;
						networkPlayers[receivedPacket.peerID].velocity = receivedPlayerData.velocity;
						networkPlayers[receivedPacket.peerID].OnGround = receivedPlayerData.onGround;
						networkPlayers[receivedPacket.peerID].walljumping = receivedPlayerData.walljumping;
					}
					break;
					case PLAYER_DISCONNECT:
					{
						HSteamNetConnection disconnectedPlayer = 0;

						memcpy(&disconnectedPlayer, receivedData + sizeof(Packet), sizeof(HSteamNetConnection));

						networkPlayers.erase(disconnectedPlayer);
					}
					break;
				}

				if (steamSocket.listenSocket > 0) // send messages to client if server
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
	else if (networkPlayers.size() > 0) // if disconnected and players remain, delete them
	{
		networkPlayers.clear();
	}
}

void GameScene::update()
{
	player.update(Time::deltaTime());

	window.camera(Vector2(player.transform.x + player.size.x / 2, (player.transform.y + player.size.y / 2) - 32));

	for (auto& [key, value] : networkPlayers)
	{
		value.update();
	}

	console.coordinatesPos = player.transform;
}

void GameScene::render(OpenGLWindow& window)
{
	for (Light light : level.lights)
	{
		window.render(light);
	}

	for (Entity wall : tiles)
	{
		window.render(wall, true);
	}

	for (Entity wall : collidableTiles)
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