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

	for (SteamNetworkingIdentity peer : peers)
	{
		SteamNetworkingMessages()->CloseSessionWithUser(peer);
		peer.Clear();
	}
	peers.clear();
}

bool NetworkManager::InLobby()
{
	return LobbyID.IsValid();
}

void NetworkManager::MessageServer(const void* message, uint32 size, int flag, int channel)
{
	CSteamID OwnerID = SteamMatchmaking()->GetLobbyOwner(LobbyID);

	if (OwnerID != SteamUser()->GetSteamID())
	{
		SteamNetworkingIdentity identity;
		identity.SetSteamID(OwnerID);

		SteamNetworkingMessages()->SendMessageToUser(identity, message, size, flag, channel);
	}
}

void NetworkManager::MessageAll(const void* message, uint32 size, int flag, int channel)
{
	SteamNetworkingIdentity identity;
	identity.SetSteamID(SteamUser()->GetSteamID());

	for (const SteamNetworkingIdentity& peer : peers)
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

			// Deserialize the byte array back into a Vector2
			if (dataSize == sizeof(Vector2))
			{
				Vector2 receivedVector;
				memcpy(&receivedVector, pData, sizeof(Vector2));

				playerPos = receivedVector;
				
				// Debug output
				char debugMsg[256];
				sprintf_s(debugMsg, "Received Vector2: x=%.2f, y=%.2f\n", receivedVector.x, receivedVector.y);
				OutputDebugStringA(debugMsg);
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

	MessageServer("", 0, k_nSteamNetworkingSend_Reliable, 0);

	CSteamID OwnerID = SteamMatchmaking()->GetLobbyOwner(LobbyID);

	PlayerJoined(OwnerID);

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

	PlayerJoined(pCallback->m_identityRemote.GetSteamID());
}

void NetworkManager::SteamNetworkingMessagesSessionFailed(SteamNetworkingMessagesSessionFailed_t* pCallback)
{
	char debugMsg[256];
	const char* username = SteamFriends()->GetFriendPersonaName(pCallback->m_info.m_identityRemote.GetSteamID());
	sprintf_s(debugMsg, "Session failed with remote peer: %s\n", username ? username : "Unknown");
	OutputDebugStringA(debugMsg);
}

void NetworkManager::PlayerJoined(CSteamID player)
{
	if (player == SteamUser()->GetSteamID()) { return; }

	SteamNetworkingIdentity identity;
	identity.SetSteamID(player);

	if (std::find(peers.begin(), peers.end(), identity) == peers.end())
	{
		OutputDebugStringA("Hello!");

		peers.push_back(identity);

		netPlayers.push_back(Player(Vector2(100, 0), playerTex, Vector2(16, 16)));
	}
}