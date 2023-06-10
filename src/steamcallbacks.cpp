#include "steamcallbacks.hpp"

void SteamCallbacks::LobbyCreated( LobbyCreated_t* pCallback )
{
	if (pCallback->m_eResult == k_EResultOK)
	{
		LobbyID = pCallback->m_ulSteamIDLobby;
	}
}