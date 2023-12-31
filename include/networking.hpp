#pragma once

#include <stddef.h>
#include <WinSock2.h>
#include <string>
#include <steam/steam_api.h>
#include <steam/isteamnetworkingsockets.h>
#include <vector>
#include <algorithm>

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

	// server
	void hostIP(std::string IPAddress);
	HSteamListenSocket listenSocket = 0;
	// client
	void connectIP(std::string IPAddress);
	HSteamNetConnection  netConnection = 0;
	// general
	void disconnect();
	void sendMessage(HSteamNetConnection peer, const uint8_t* data, uint32_t dataSize, int sendFlags);
	int receiveMessages(HSteamNetConnection peer, SteamNetworkingMessage_t** messages, int maxMessages);
	std::vector<HSteamNetConnection> peers;
private:
	//SteamNetworkingMessage_t* messages[MAX_MESSAGES] = {};

	STEAM_CALLBACK(SteamSocket, SteamNetConnectionStatusChangedCallback, SteamNetConnectionStatusChangedCallback_t);
};

extern SteamSocket steamSocket;