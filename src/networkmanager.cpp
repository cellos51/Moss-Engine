#include "networkmanager.hpp"
#include <Windows.h>
#include <string>

// constructor / destructor

NetworkManager::NetworkManager()
{

}

NetworkManager::~NetworkManager()
{

}

// functions

void NetworkManager::CreateLobby()
{
	LeaveLobby();
	SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, MAX_PLAYERS);
}

void NetworkManager::LeaveLobby()
{
	if (InLobby())
	{
		SteamMatchmaking()->LeaveLobby(LobbyID);
	}

	for (auto& [key, peer] : peers)
	{
		SteamNetworkingMessages()->CloseSessionWithUser(peer);
		peer.Clear();
	}
	playerIndex = 0;
	netPlayers.clear();
	peers.clear();
}

bool NetworkManager::InLobby()
{
	return LobbyID.IsValid();
}

void NetworkManager::PingServer()
{
	CSteamID OwnerID = SteamMatchmaking()->GetLobbyOwner(LobbyID);
	CSteamID LocalID = SteamUser()->GetSteamID();

	if (LocalID == OwnerID) { return; }

	SteamNetworkingIdentity identity;
	identity.SetSteamID(OwnerID);

	SteamNetworkingMessages()->SendMessageToUser(identity, "", 0, k_nSteamNetworkingSend_Reliable, 0);
}

void NetworkManager::MessageServer(const void* message, uint32 size, int flag, int channel)
{
	CSteamID OwnerID = SteamMatchmaking()->GetLobbyOwner(LobbyID);
	CSteamID LocalID = SteamUser()->GetSteamID();

	if (playerIndex == 0 || LocalID == OwnerID) { return; }
		
	SteamNetworkingIdentity identity;
	identity.SetSteamID(OwnerID);

	SteamNetworkingMessages()->SendMessageToUser(identity, message, size, flag, channel);
}

void NetworkManager::MessageAll(const void* message, uint32 size, int flag, int channel)
{
	CSteamID LocalID = SteamUser()->GetSteamID();

	if (playerIndex == 0 && LocalID != SteamMatchmaking()->GetLobbyOwner(LobbyID)) { return; }

	SteamNetworkingIdentity identity;
	identity.SetSteamID(LocalID);

	for (const auto& [key, peer] : peers)
	{
		if (peer != identity)
		{
			SteamNetworkingMessages()->SendMessageToUser(peer, message, size, flag, channel);
		}
	}
}

void NetworkManager::ReceiveMessages()
{
	int nReceivedMessages = SteamNetworkingMessages()->ReceiveMessagesOnChannel(0, pMessages, MAX_MESSAGES);

	if (nReceivedMessages > 0)
	{
		// Messages were received successfully
		for (int i = 0; i < nReceivedMessages; i++)
		{
			const uint8_t* pData = static_cast<const uint8_t*>(pMessages[i]->GetData());
			uint32_t dataSize = pMessages[i]->GetSize();

			if (dataSize == sizeof(unsigned int))
			{
				memcpy(&playerIndex, pData, sizeof(unsigned int));

				playerIndex = 1;

				char debugMsg[256];
				sprintf_s(debugMsg, "Index received: %u\n", playerIndex);
				OutputDebugStringA(debugMsg);
			}
			else if (dataSize == sizeof(PlayerData))
			{
				PlayerData receivedData;
				memcpy(&receivedData, pData, sizeof(PlayerData));

				netPlayers.find(receivedData.index)->second.transform = receivedData.position;
			}

			// Process the received message(s) as needed
			pMessages[i]->Release();
		}
	}
}

// callbacks

void NetworkManager::LobbyCreated(LobbyCreated_t* pCallback)
{
	if (pCallback->m_eResult == k_EResultOK)
	{
		OutputDebugString("Lobby created. \n");
	}
}

void NetworkManager::GameLobbyJoinRequested(GameLobbyJoinRequested_t* pCallback)
{
	if (pCallback->m_steamIDLobby.IsValid())
	{
		SteamMatchmaking()->JoinLobby(pCallback->m_steamIDLobby);
	}	
}

void NetworkManager::LobbyEnter(LobbyEnter_t* pCallback)
{
	LobbyID = pCallback->m_ulSteamIDLobby;

	PingServer();

	CSteamID OwnerID = SteamMatchmaking()->GetLobbyOwner(LobbyID);

	AddPeer(OwnerID);

	OutputDebugString("Lobby entered. \n");
}

void NetworkManager::LobbyChatUpdate(LobbyChatUpdate_t* pCallback)
{
	char debugMsg[256];
	CSteamID playerID;
	const char* playerName;

		// Check the lobby chat state change
		switch (pCallback->m_rgfChatMemberStateChange)
		{
		case k_EChatMemberStateChangeEntered:
			// A player joined the lobby
			playerID = pCallback->m_ulSteamIDUserChanged;

			playerName = SteamFriends()->GetFriendPersonaName(playerID);
			sprintf_s(debugMsg, "%s has joined the lobby. \n", playerName ? playerName : "Unknown");
			OutputDebugStringA(debugMsg);

			break;
		case k_EChatMemberStateChangeLeft:
			// A player left the lobby
			playerID = pCallback->m_ulSteamIDUserChanged;

			playerName = SteamFriends()->GetFriendPersonaName(playerID);
			sprintf_s(debugMsg, "%s has left the lobby. \n", playerName ? playerName : "Unknown");
			OutputDebugStringA(debugMsg);

			break;
			// Handle other lobby chat state changes if needed
		}
}

void NetworkManager::SteamNetworkingMessagesSessionRequest(SteamNetworkingMessagesSessionRequest_t *pCallback)
{
	char debugMsg[256];
	const char* username = SteamFriends()->GetFriendPersonaName(pCallback->m_identityRemote.GetSteamID());
	sprintf_s(debugMsg, "Message session request from %s accepted.\n", username ? username : "Unknown");
	OutputDebugStringA(debugMsg);

	SteamNetworkingMessages()->AcceptSessionWithUser(pCallback->m_identityRemote);

	AddPeer(pCallback->m_identityRemote.GetSteamID());

	for (const auto& [key, peer] : peers)
	{
		if (peer == pCallback->m_identityRemote)
		{
			uint32_t dataSize = sizeof(unsigned int);
			uint8_t* pData = new uint8_t[dataSize];
			memcpy(pData, &key, sizeof(float));

			SteamNetworkingMessages()->SendMessageToUser(peer, pData, dataSize, k_nSteamNetworkingSend_Reliable, 0);
		}
	}
}

void NetworkManager::SteamNetworkingMessagesSessionFailed(SteamNetworkingMessagesSessionFailed_t* pCallback)
{
	char debugMsg[256];
	const char* username = SteamFriends()->GetFriendPersonaName(pCallback->m_info.m_identityRemote.GetSteamID());
	sprintf_s(debugMsg, "Session failed with remote peer: %s\n", username ? username : "Unknown");
	OutputDebugStringA(debugMsg);
}

void NetworkManager::AddPeer(CSteamID player)
{
	SteamNetworkingIdentity identity;
	identity.SetSteamID(player);

	unsigned int index = 0;
	while (peers.find(index) != peers.end())
	{
		++index;
	}

	peers.emplace(index, identity);

	if (player != SteamUser()->GetSteamID())
	{
		netPlayers.emplace(index, Player(Vector2(0, 0), playerTex, Vector2(16, 16)));
		netPlayers.find(index)->second.layer = 3;
	}
}