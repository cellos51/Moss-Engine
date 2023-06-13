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
	if (!InLobby())
	{
		SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, MAX_PLAYERS);
	}
}

bool NetworkManager::InLobby()
{
	return LobbyID.IsValid();
}

void NetworkManager::MessageServer(const char* message, int flag, int channel)
{
	CSteamID OwnerID = SteamMatchmaking()->GetLobbyOwner(LobbyID);

	if (OwnerID != SteamUser()->GetSteamID())
	{
		SteamNetworkingIdentity identity;
		identity.SetSteamID(OwnerID);

		SteamNetworkingMessages()->SendMessageToUser(identity, message, strlen(message), flag, channel);
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
			const char* pStringData = static_cast<const char*>(pMessages[i]->GetData());

			OutputDebugString(pStringData);

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
}

void NetworkManager::SteamNetworkingMessagesSessionFailed(SteamNetworkingMessagesSessionFailed_t* pCallback)
{
	char debugMsg[256];
	const char* username = SteamFriends()->GetFriendPersonaName(pCallback->m_info.m_identityRemote.GetSteamID());
	sprintf_s(debugMsg, "Session failed with remote peer: %s\n", username ? username : "Unknown");
	OutputDebugStringA(debugMsg);
}