#pragma once

#include <stddef.h>
#include <steam/steam_api.h>

class SteamCallbacks
{
public:
	CSteamID LobbyID;
private:
	STEAM_CALLBACK(SteamCallbacks, LobbyCreated, LobbyCreated_t);
};