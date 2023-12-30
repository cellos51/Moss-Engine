#include "networking.hpp"
#include "console.hpp"

SteamSocket steamSocket;

SteamSocket::SteamSocket()
{

}

SteamSocket::~SteamSocket()
{

}

// server stuff

void SteamSocket::hostIP(std::string IPAddress)
{
	disconnect();

	console.log("server created on " + IPAddress + "\n");

	SteamNetworkingIPAddr hostAddress;
	hostAddress.SetIPv4(ntohl(inet_addr(IPAddress.c_str())), 25565);
	listenSocket = SteamNetworkingSockets()->CreateListenSocketIP(hostAddress, 0, 0);
}

// client stuff

void SteamSocket::connectIP(std::string IPAddress)
{
	disconnect();

	console.log("connecting to " + IPAddress + "\n");

	SteamNetworkingIPAddr hostAddress;
	hostAddress.SetIPv4(ntohl(inet_addr(IPAddress.c_str())), 25565);

	netConnection = SteamNetworkingSockets()->ConnectByIPAddress(hostAddress, 0, 0);

	SDL_Delay(300); // this needs to be here for some reason
}

void SteamSocket::messageServer()
{
	std::string message = "the quick brown message jumps over the lazy packet";

	uint32_t dataSize = message.length();
	uint8_t* data = new uint8_t[dataSize];

	memcpy(data, message.data(), dataSize);

	int64 debugNumber;

	EResult result = SteamNetworkingSockets()->SendMessageToConnection(peers[0], data, dataSize, k_nSteamNetworkingSend_Reliable, &debugNumber);

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
	default:
		console.log("message sent \n");
		break;
	}
}

// general 

void SteamSocket::disconnect()
{
	SteamNetworkingSockets()->CloseListenSocket(listenSocket); // server
	SteamNetworkingSockets()->CloseConnection(netConnection, 0, "disconnect function called", false); // client
	peers.clear();
}

void SteamSocket::receiveMessages()
{
	if (peers.size() > 0)
	{
		int receivedMessages = SteamNetworkingSockets()->ReceiveMessagesOnConnection(peers[0], messages, 8);

		if (receivedMessages > 0)
		{
			console.log("message got \n");

			for (int i = 0; i < receivedMessages; i++)
			{
				const uint8_t* messageData = static_cast<const uint8_t*>(messages[i]->GetData());
				uint32_t messageSize = messages[i]->GetSize();
				
				// wip

				std::string message(messageData, messageData + messageSize);

				console.log(message + "\n");

				messages[i]->Release();
			}
		}
	}
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