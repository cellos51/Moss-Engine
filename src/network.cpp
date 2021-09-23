#include "network.hpp"

#include <iostream>
#include <string>
#include <set>


ENetAddress address;
ENetHost* server;
ENetHost* client;
ENetEvent event;
ENetPeer* peer;
ENetPacket* packet;
bool isServer = false;

int CLIENT_ID = 0;

int TEST_ID;

std::set<int> connectedClients;

float x, y = 0;

// Global functions

void sendData(ENetPeer* peers, int data_type, int id, std::string data)
{
	std::string packatData = std::to_string(data_type) + " " + std::to_string(id) + " " + data;
	packet = enet_packet_create (packatData.c_str(), strlen (packatData.c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send (peers, 0, packet);
}

void broadcastData(ENetHost* servers, int data_type, int id, std::string data)
{
	std::string packatData = std::to_string(data_type) + " " + std::to_string(id) + " " + data;
	packet = enet_packet_create (packatData.c_str(), strlen (packatData.c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast (servers, 0, packet);
}

void parseData(unsigned char* data)
{
	const char *char_pointer = (char*)data;
	

	int data_type;
	int id;

	sscanf(char_pointer, "%d %d", &data_type, &id);

	switch(data_type)
	{
		case 0:
			if (CLIENT_ID == 0)
			{
				CLIENT_ID = id;
				std::cout << "I am client " << CLIENT_ID << "\n";
			}
			break;
		case 1:
			if (CLIENT_ID != id)
			{
				sscanf(char_pointer, "%*d %d %f %f", &TEST_ID, &x, &y);
				if (isServer == true)
				{
					broadcastData(server, 1, TEST_ID, std::to_string(x) + " " + std::to_string(y));
				}
			}
			break;
		case 2:
			int cID;
			sscanf(char_pointer, "%*d %*d %d", &cID);
			if (cID != CLIENT_ID)
			{
				connectedClients.insert(cID);
				std::cout << cID << "\n";
			}
			break;
	}
}

// Beginning of Server

Server::Server()
{

}

void Server::create()
{
	isServer = true;
	address.host = ENET_HOST_ANY;
	address.port = 1234;
	server = enet_host_create (&address, 32, 1, 0, 0); 

	if (server == NULL)
	{
	    std::cout << "An error occurred while trying to create an ENet server host.\n";
	    exit (EXIT_FAILURE);
	}
}

void Server::poll()
{
	while (enet_host_service (server, &event, 0) > 0)
	{
		switch(event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				std::cout << "Client connected.\n";

				SERVER_ID++;
				connectedClients.insert(SERVER_ID);
				sendData(event.peer, 0, SERVER_ID, " ");

				for (auto f : connectedClients)
				{
					broadcastData(server, 2, SERVER_ID, std::to_string(f));
				}

				break;
			case ENET_EVENT_TYPE_RECEIVE:
				parseData(event.packet->data);
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout << "Disconnected.\n";
			case ENET_EVENT_TYPE_NONE:
				break;
		}
	}
}

Vector2 Server::getPacket()
{
	return Vector2(x,y);
}

int Server::getId()
{
	return TEST_ID;
}

void Server::sendPacket(Vector2 fuck)
{
	broadcastData(server, 1, 0, std::to_string(fuck.x) + " " + std::to_string(fuck.y));
}

void Server::destroy()
{
	enet_host_destroy(server);
}

// Beginning of Client

Client::Client()
{

}

void Client::connect()
{
	isServer = false;
	client = enet_host_create (NULL, 1, 1, 0, 0);

	if (client == NULL)
	{
	    std::cout << "An error occurred while trying to create an ENet client host.\n";
	    exit (EXIT_FAILURE);
	}

	enet_address_set_host(&address, "127.0.0.1");
	address.port = 1234;
	peer = enet_host_connect (client, &address, 1, 0);
	if (peer == NULL)
	{
		std::cout << "No available peers for initiating an ENet connection.\n";
		exit (EXIT_FAILURE);
	}

	if (enet_host_service (client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
	    std::cout << "Connection to some.server.net:1234 succeeded.\n";
	}
	else
	{
	    enet_peer_reset (peer);
	    std::cout << "Connection to some.server.net:1234 failed.\n";
	}
}

void Client::disconnect()
{
	enet_peer_disconnect(peer, 0);
}

void Client::poll()
{
	while (enet_host_service (client, &event, 0) > 0)
	{
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				std::cout << "Connected.\n";
				break;
			case ENET_EVENT_TYPE_RECEIVE:
		        parseData(event.packet->data);
		        enet_packet_destroy (event.packet);   
	        	break;
	        case ENET_EVENT_TYPE_DISCONNECT:
	        	std::cout << "Disconnected.\n";
	        	enet_host_destroy(client);
	        	break;
	        case ENET_EVENT_TYPE_NONE:
				break;
		}
	}
}

Vector2 Client::getPacket()
{
	return Vector2(x,y);
}

int Client::getId()
{
	return TEST_ID;
}

void Client::sendPacket(Vector2 fuck)
{
	if (CLIENT_ID != 0)
	{
		sendData(peer, 1, CLIENT_ID, std::to_string(fuck.x) + " " + std::to_string(fuck.y));
	}
}