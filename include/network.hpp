#pragma once

#include <string>
#include <set>
#include <vector>

#include "enet/enet.h"

#include "math.hpp"

namespace Net
{
	void serverCreate();
	void clientConnect(std::string ipAdress);
	void clientDisconnect();
	void serverDestroy();
	void poll();
	Vector2 getPacket();
	int getId();
	std::vector<int> allPlayers();
	void sendPacket(Vector2 pos);
}