#include "networkmanager.hpp"
#include <Windows.h>

NetworkManager::NetworkManager()
{

}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::CreateLobby()
{
	SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, MAX_PLAYERS);
}

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
		OutputDebugString("Lobby joining. \n");
	}	
}

void NetworkManager::LobbyEnter(LobbyEnter_t* pCallback)
{
	OutputDebugString("Lobby entered. \n");
}