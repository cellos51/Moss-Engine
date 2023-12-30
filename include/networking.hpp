#pragma once

#include <stddef.h>
#include <WinSock2.h>
#include <string>
#include <steam/steam_api.h>
#include <steam/isteamnetworkingsockets.h>
#include <vector>
#include <algorithm>

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
	void messageServer();
	HSteamNetConnection  netConnection = 0;
	// general
	void disconnect();
	void receiveMessages();
	std::vector<HSteamNetConnection> peers;
private:
	SteamNetworkingMessage_t* messages[8] = {};

	STEAM_CALLBACK(SteamSocket, SteamNetConnectionStatusChangedCallback, SteamNetConnectionStatusChangedCallback_t);
};

extern SteamSocket steamSocket;