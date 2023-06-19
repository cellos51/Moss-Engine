#pragma once

#include <stddef.h>
#include <steam/steam_api.h>
#include <steam/isteamnetworkingmessages.h>
#include <steam/isteammatchmaking.h>
#include <steam/isteamgameserver.h>

#include <map>
#include "networkplayer.hpp"

#define MAX_PLAYERS 4

#define MAX_MESSAGES 8

struct PlayerData
{
	unsigned int index = 0;
	Vector2 position;
	unsigned int movement = 0; // 0 standing, 1 walking, 2 running.
	unsigned int direction = 0; // 1 up, 2 down, 3 left, 4 right.
};

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();
	void CreateLobby();
	void LeaveLobby();
	bool InLobby();
	void PingServer();
	void MessageServer(const void* message, uint32 size, int flag, int channel); // just for testing :P
	void MessageAll(const void* message, uint32 size, int flag, int channel); // just for testing :P
	void ReceiveMessages();

	unsigned int playerTex = 0;

	std::map<unsigned int, NetworkPlayer> netPlayers;
	unsigned int playerIndex = 0;
private:
	void AddPeer(CSteamID player);

	CSteamID LobbyID;
	SteamNetworkingMessage_t* pMessages[MAX_MESSAGES] = {};
	std::map<unsigned int, SteamNetworkingIdentity> peers;

	STEAM_CALLBACK(NetworkManager, LobbyCreated, LobbyCreated_t);
	STEAM_CALLBACK(NetworkManager, GameLobbyJoinRequested, GameLobbyJoinRequested_t);
	STEAM_CALLBACK(NetworkManager, LobbyEnter, LobbyEnter_t);	
	STEAM_CALLBACK(NetworkManager, LobbyChatUpdate, LobbyChatUpdate_t);
	STEAM_CALLBACK(NetworkManager, SteamNetworkingMessagesSessionRequest, SteamNetworkingMessagesSessionRequest_t);
	STEAM_CALLBACK(NetworkManager, SteamNetworkingMessagesSessionFailed, SteamNetworkingMessagesSessionFailed_t);
};