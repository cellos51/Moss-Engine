#pragma once

#include <stddef.h>
#include <steam/steam_api.h>
#include <steam/isteamnetworkingmessages.h>
#include <steam/isteammatchmaking.h>
#include <steam/isteamgameserver.h>

#define MAX_PLAYERS 4

#define MAX_MESSAGES 10

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();
	void CreateLobby();
	bool InLobby();
	void MessageServer(const char* message, int flag, int channel); // just for testing :P
	void ReceiveMessages();
private:
	CSteamID LobbyID;
	SteamNetworkingMessage_t* pMessages[MAX_MESSAGES] = {};

	STEAM_CALLBACK(NetworkManager, LobbyCreated, LobbyCreated_t);
	STEAM_CALLBACK(NetworkManager, GameLobbyJoinRequested, GameLobbyJoinRequested_t);
	STEAM_CALLBACK(NetworkManager, LobbyEnter, LobbyEnter_t);	
	STEAM_CALLBACK(NetworkManager, LobbyChatUpdate, LobbyChatUpdate_t);
	STEAM_CALLBACK(NetworkManager, SteamNetworkingMessagesSessionRequest, SteamNetworkingMessagesSessionRequest_t);
	STEAM_CALLBACK(NetworkManager, SteamNetworkingMessagesSessionFailed, SteamNetworkingMessagesSessionFailed_t);
};