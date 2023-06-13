#pragma once

#include <stddef.h>
#include <steam/steam_api.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteammatchmaking.h>
#include <steam/isteamgameserver.h>

#define MAX_PLAYERS 4

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();
	void CreateLobby();
private:
	HSteamListenSocket ListenSocket;
	HSteamNetPollGroup NetPollGroup;

	STEAM_CALLBACK(NetworkManager, LobbyCreated, LobbyCreated_t);
	STEAM_CALLBACK(NetworkManager, GameLobbyJoinRequested, GameLobbyJoinRequested_t);
	STEAM_CALLBACK(NetworkManager, LobbyEnter, LobbyEnter_t);	
};