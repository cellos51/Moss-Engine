#pragma once

#include <string>
#include <set>

#include "enet/enet.h"

#include "math.hpp"

struct Server
{
	Server();
	void create();
	void poll();
	Vector2 getPacket();
	int getId();
	void sendPacket(Vector2 fuck);
	void destroy();
	int SERVER_ID = 0;
};

struct Client
{
	Client();
	void connect();
	void disconnect();
	void poll();
	Vector2 getPacket();
	int getId();
	void sendPacket(Vector2 fuck);
};