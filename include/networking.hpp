#pragma once

#include <stddef.h>
#include <WinSock2.h>
#include <steam/steam_api.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteammatchmaking.h>
#include <steam/isteamfriends.h>

#include <string>
#include <vector>
#include <algorithm>


#define MAX_PLAYERS 4

#define PORT 27015

enum PacketType : uint8_t
{
	PLAYER_DATA,
};

struct Packet
{
	unsigned long int peerID;
	PacketType packetType;
};

class SteamSocket
{
public:
	SteamSocket();
	~SteamSocket();
	void init();

	// server
	void hostIP(std::string IPAddress);
	void hostP2P();
	HSteamListenSocket listenSocket = 0;
	// client
	void connectIP(std::string IPAddress);
	void connectP2P(SteamNetworkingIdentity identity);
	HSteamNetConnection  netConnection = 0;
	// general
	void disconnect();
	void sendMessage(HSteamNetConnection peer, const uint8_t* data, uint32_t dataSize, int sendFlags);
	int receiveMessages(HSteamNetConnection peer, SteamNetworkingMessage_t** messages, int maxMessages);
	std::vector<HSteamNetConnection> peers;
private:
	SteamNetworkingConfigValue_t connectionConfig;
	uint64 lobbyID = 0;
	//SteamNetworkingMessage_t* messages[MAX_MESSAGES] = {};

	STEAM_CALLBACK(SteamSocket, SteamNetConnectionStatusChangedCallback, SteamNetConnectionStatusChangedCallback_t);
	STEAM_CALLBACK(SteamSocket, GameLobbyJoinRequested, GameLobbyJoinRequested_t);
	STEAM_CALLBACK(SteamSocket, LobbyEnter, LobbyEnter_t);
};

extern SteamSocket steamSocket;