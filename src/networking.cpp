#include "networking.hpp"
#include "console.hpp"

SteamSocket steamSocket;

SteamSocket::SteamSocket()
{

}

SteamSocket::~SteamSocket()
{
	//disconnect();
}

// server stuff

void SteamSocket::hostIP(std::string IPAddress)
{
	disconnect();

	console.log("server created on " + IPAddress + "\n");

	SteamNetworkingIPAddr hostAddress;
	hostAddress.SetIPv4(ntohl(inet_addr(IPAddress.c_str())), 7777);

	SteamNetworkingConfigValue_t config{ k_ESteamNetworkingConfig_IP_AllowWithoutAuth, k_ESteamNetworkingConfig_Int32, 1 };

	listenSocket = SteamNetworkingSockets()->CreateListenSocketIP(hostAddress, 1, &config);
}

// client stuff

void SteamSocket::connectIP(std::string IPAddress)
{
	disconnect();

	console.log("connecting to " + IPAddress + "\n");

	SteamNetworkingIPAddr hostAddress;
	hostAddress.SetIPv4(ntohl(inet_addr(IPAddress.c_str())), 7777);

	SteamNetworkingConfigValue_t config{ k_ESteamNetworkingConfig_IP_AllowWithoutAuth, k_ESteamNetworkingConfig_Int32, 1 };

	netConnection = SteamNetworkingSockets()->ConnectByIPAddress(hostAddress, 1, &config);

	SDL_Delay(300); // this needs to be here for some reason
}

// general 

void SteamSocket::disconnect()
{
	SteamNetworkingSockets()->CloseListenSocket(listenSocket); // server
	SteamNetworkingSockets()->CloseConnection(netConnection, 0, "disconnect function called", false); // client
	peers.clear();


}

void SteamSocket::sendMessage(HSteamNetConnection peer, const uint8_t* data, uint32_t dataSize, int sendFlags)
{
	int64 debugNumber;

	EResult result = SteamNetworkingSockets()->SendMessageToConnection(peer, data, dataSize, sendFlags, &debugNumber);

	//console.log(std::to_string(debugNumber) + "\n");
	//console.log(std::to_string(k_EResultLimitExceeded) + "\n");

	switch (result)
	{
	case k_EResultInvalidParam:
		console.log("invalid param \n");
		break;
	case k_EResultInvalidState:
		console.log("invalid state \n");
		break;
	case k_EResultNoConnection:
		console.log("no connection \n");
		break;
	case k_EResultIgnored:
		console.log("ignored \n");
		break;
	case k_EResultLimitExceeded:
		console.log("limit exceeded \n");
		break;
	}
}

int SteamSocket::receiveMessages(HSteamNetConnection peer, SteamNetworkingMessage_t** messages, int maxMessages)
{
		return SteamNetworkingSockets()->ReceiveMessagesOnConnection(peer, messages, maxMessages);
}

// callbacks

void SteamSocket::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pCallback)
{
	console.log("connection status changed!" + std::to_string(pCallback->m_info.m_eState) + "\n");

	if (std::find(peers.begin(), peers.end(), pCallback->m_hConn) == peers.end()) // if first contact with new connection
	{
		peers.push_back(pCallback->m_hConn);
		console.log("peer added \n");

		if (listenSocket > 0) // if server
		{
			EResult result = SteamNetworkingSockets()->AcceptConnection(pCallback->m_hConn);

			switch (result)
			{
			case k_EResultInvalidParam:
				console.log("invalid param \n");
				break;
			case k_EResultInvalidState:
				console.log("invalid state \n");
				break;
			case k_EResultOK:
				console.log("connection accepted fine \n");
				break;
			default:
				console.log("idk the error. here is a number ig: " + std::to_string(result) + "\n");
				break;
			}
		}
	}
}