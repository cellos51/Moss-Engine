#pragma once

#include <stddef.h>
#include <steam/steam_api.h>
#include <steam/isteamnetworkingmessages.h>
#include <steam/isteammatchmaking.h>
#include <steam/isteamgameserver.h>

#include <map>
#include "player.hpp"

#define MAX_PLAYERS 4

#define MAX_MESSAGES 8

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();
	void CreateLobby();
	void LeaveLobby();
	bool InLobby();
	void MessageServer(const void* message, uint32 size, int flag, int channel); // just for testing :P
	void MessageAll(const void* message, uint32 size, int flag, int channel); // just for testing :P
	void ReceiveMessages();

	unsigned int playerTex = 0;

	Vector2 playerPos;

	std::vector<Player> netPlayers;
private:
	void PlayerJoined(CSteamID player);

	CSteamID LobbyID;
	SteamNetworkingMessage_t* pMessages[MAX_MESSAGES] = {};
	std::vector<SteamNetworkingIdentity> peers;

	STEAM_CALLBACK(NetworkManager, LobbyCreated, LobbyCreated_t);
	STEAM_CALLBACK(NetworkManager, GameLobbyJoinRequested, GameLobbyJoinRequested_t);
	STEAM_CALLBACK(NetworkManager, LobbyEnter, LobbyEnter_t);	
	STEAM_CALLBACK(NetworkManager, LobbyChatUpdate, LobbyChatUpdate_t);
	STEAM_CALLBACK(NetworkManager, SteamNetworkingMessagesSessionRequest, SteamNetworkingMessagesSessionRequest_t);
	STEAM_CALLBACK(NetworkManager, SteamNetworkingMessagesSessionFailed, SteamNetworkingMessagesSessionFailed_t);
};