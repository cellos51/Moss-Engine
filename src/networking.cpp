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

void init()
{
	SteamNetworkingUtils()->InitRelayNetworkAccess();
}

// server stuff

void SteamSocket::hostIP(std::string IPAddress)
{
	disconnect();

	SteamNetworkingIPAddr hostAddress; hostAddress.Clear();
	hostAddress.SetIPv4(ntohl(inet_addr(IPAddress.c_str())), port);
	
	console.log("server created \n");

	SteamNetworkingConfigValue_t config{ k_ESteamNetworkingConfig_IP_AllowWithoutAuth, k_ESteamNetworkingConfig_Int32, 1 };

	SteamMatchmaking()->CreateLobby(k_ELobbyTypeFriendsOnly, MAX_PLAYERS);

	listenSocket = SteamNetworkingSockets()->CreateListenSocketIP(hostAddress, 1, &config);

	SteamMatchmaking()->SetLobbyGameServer(lobbyID, hostAddress.GetIPv4(), hostAddress.m_port, k_steamIDNil);
}

void SteamSocket::hostP2P()
{
	disconnect();

	SteamNetworkingIPAddr hostAddress; hostAddress.Clear();
	hostAddress.SetIPv4(0, port);

	console.log("peer to peer server created \n");

	SteamNetworkingConfigValue_t config{ k_ESteamNetworkingConfig_IP_AllowWithoutAuth, k_ESteamNetworkingConfig_Int32, 1 };

	SteamMatchmaking()->CreateLobby(k_ELobbyTypeFriendsOnly, MAX_PLAYERS);

	listenSocket = SteamNetworkingSockets()->CreateListenSocketP2P(0, 1, &config);
}

// client stuff

void SteamSocket::connectIP(std::string IPAddress)
{
	disconnect();

	console.log("connecting to " + IPAddress + "\n");

	SteamNetworkingIPAddr hostAddress; hostAddress.Clear();
	hostAddress.SetIPv4(ntohl(inet_addr(IPAddress.c_str())), port);

	SteamNetworkingConfigValue_t config{ k_ESteamNetworkingConfig_IP_AllowWithoutAuth, k_ESteamNetworkingConfig_Int32, 1 };

	netConnection = SteamNetworkingSockets()->ConnectByIPAddress(hostAddress, 1, &config);

	SDL_Delay(300); // this needs to be here for some reason
}

void SteamSocket::connectP2P(SteamNetworkingIdentity identity)
{
	disconnect();

	console.log("connecting to some steam user (i didn't add a way to check yet) \n");

	SteamNetworkingConfigValue_t config{ k_ESteamNetworkingConfig_IP_AllowWithoutAuth, k_ESteamNetworkingConfig_Int32, 1 };

	netConnection = SteamNetworkingSockets()->ConnectP2P(identity, 0, 1, &config);

	SDL_Delay(300); // this needs to be here for some reason
}

// general 

void SteamSocket::disconnect()
{
	SteamMatchmaking()->LeaveLobby(lobbyID);
	SteamNetworkingSockets()->CloseListenSocket(listenSocket); // server
	SteamNetworkingSockets()->CloseConnection(netConnection, k_ESteamNetworkingConnectionState_ClosedByPeer, nullptr, false); // client
	peers.clear();

	console.log("disconnected \n");
}

void SteamSocket::sendMessage(HSteamNetConnection peer, const uint8_t* data, uint32_t dataSize, int sendFlags)
{
	int64 debugNumber;

	EResult result = SteamNetworkingSockets()->SendMessageToConnection(peer, data, dataSize, sendFlags, &debugNumber);

	//console.log(std::to_string(debugNumber) + "\n");
	//console.log(std::to_string(k_EResultLimitExceeded) + "\n");

	/*switch (result)
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
	}*/
}

int SteamSocket::receiveMessages(HSteamNetConnection peer, SteamNetworkingMessage_t** messages, int maxMessages)
{
		return SteamNetworkingSockets()->ReceiveMessagesOnConnection(peer, messages, maxMessages);
}

// callbacks

void SteamSocket::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* pCallback)
{
	if (std::find(peers.begin(), peers.end(), pCallback->m_hConn) == peers.end() && pCallback->m_info.m_eState == k_ESteamNetworkingConnectionState_Connecting) // if first contact with new connection
	{
		peers.push_back(pCallback->m_hConn);
		console.log("peer added \n");

		if (listenSocket > 0) // if server
		{
			EResult result = SteamNetworkingSockets()->AcceptConnection(pCallback->m_hConn);

			/*switch (result)
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
			}*/
		}
	}

	std::string peerName = SteamFriends()->GetFriendPersonaName(pCallback->m_info.m_identityRemote.GetSteamID());

	switch (pCallback->m_info.m_eState)
	{
	case k_ESteamNetworkingConnectionState_Connecting:
		console.log(peerName + " connecting... \n");
		break;
	case k_ESteamNetworkingConnectionState_FindingRoute:
		console.log(peerName + " finding route... \n");
		break;
	case k_ESteamNetworkingConnectionState_Connected:
		console.log(peerName + " connected! \n");
		break;
	case k_ESteamNetworkingConnectionState_ClosedByPeer:
		console.log(peerName + " disconnected \n");

		peers.erase(std::remove(peers.begin(), peers.end(), pCallback->m_hConn), peers.end());
		break;
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
		console.log("network error! \n");
		disconnect();
		break;
	case k_ESteamNetworkingConnectionState_None:
		// don't do anything for this one
		break;
	default:
		console.log("unknown connection status error: " + std::to_string(pCallback->m_info.m_eState) + " \n");
		break;
	}
}


void SteamSocket::GameLobbyJoinRequested(GameLobbyJoinRequested_t* pCallback)
{
	if (pCallback->m_steamIDLobby.IsValid())
	{
		SteamMatchmaking()->JoinLobby(pCallback->m_steamIDLobby);

		SteamNetworkingIdentity identity;
		identity.SetSteamID(SteamMatchmaking()->GetLobbyOwner(SteamMatchmaking()->GetLobbyOwner(lobbyID)));

		connectP2P(identity);
	}
}

void SteamSocket::LobbyEnter(LobbyEnter_t* pCallback)
{
	lobbyID = pCallback->m_ulSteamIDLobby;

	console.log("joined lobby \n");
}