#include "network.hpp"

#include <enet/enet.h>
#include <iostream>
#include <string>
#include <set>
#include <vector>

//this is probably a shit show and optimizing it right now is not worth it

ENetAddress address;
ENetHost* server;
ENetHost* client;
ENetEvent event;
ENetPeer* peer;
ENetPacket* packet;
bool isServer = false;

int CLIENT_ID = 0;

int packetID = 0;

std::set<int> connectedClients;

float x, y = 0;



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
		case 0: // on connect
			if (CLIENT_ID == 0)
			{
				CLIENT_ID = id;
				std::cout << "Client ID is " << CLIENT_ID << std::endl;
			}
			break;
		case 1: // player pos
			if (CLIENT_ID != id) 
			{
				sscanf(char_pointer, "%*d %d %f %f", &packetID, &x, &y);
				if (isServer == true)
				{
					broadcastData(server, 1, packetID, std::to_string(x) + " " + std::to_string(y));
				}
			}
			break;
		case 2: // I FORGOR i think this puts players in a vector or somthing idk why this is useless
			int cID; 
			sscanf(char_pointer, "%*d %*d %d", &cID);
			if (cID != CLIENT_ID && !connectedClients.count(cID))
			{
				connectedClients.insert(cID);
				std::cout << "Client " << cID << " connected.\n";
			}
			break; 
		case 3:	// disconnecting players
			sscanf(char_pointer, "%*d %*d %d", &cID);
			connectedClients.erase(cID);
			std::cout << "Client " << cID << " disconnected." << std::endl;
			break;
	}
}

void Net::serverCreate()
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
	else
	{
		std::cout << "Server created on: " << address.host << ":" << address.port << std::endl;
	}
}

void Net::clientConnect(std::string ipAdress)
{
	isServer = false;
	client = enet_host_create (NULL, 1, 1, 0, 0);

	if (client == NULL)
	{
	    std::cout << "An error occurred while trying to create an ENet client host.\n";
	    exit (EXIT_FAILURE);
	}

	enet_address_set_host(&address, ipAdress.c_str());
	address.port = 1234;
	peer = enet_host_connect (client, &address, 1, 0);
	if (peer == NULL)
	{
		std::cout << "No available peers for initiating an ENet connection.\n";
		exit (EXIT_FAILURE);
	}

	if (enet_host_service (client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
	{
	    std::cout << "Connection to " << address.host << ":" << address.port << " succeeded.\n";
	}
	else
	{
	    enet_peer_reset (peer);
	    std::cout << "Connection to " << address.host << ":" << address.port << " failed.\n";
	}
}

void Net::clientDisconnect()
{
	enet_peer_disconnect(peer, 0);
}

void Net::serverDestroy()
{
	enet_host_destroy(server);
}

void Net::poll()
{
	if (isServer == true)
	{
		while (enet_host_service (server, &event, 0) > 0)
		{
			switch(event.type)
			{
				case ENET_EVENT_TYPE_CONNECT:
					std::cout << "Client " << event.peer -> incomingPeerID + 1 << " connected.\n";

					connectedClients.insert(event.peer -> incomingPeerID + 1);
					sendData(event.peer, 0, event.peer -> incomingPeerID + 1, " ");

					broadcastData(server, 2, event.peer -> incomingPeerID + 1, std::to_string(0));
					for (auto f : connectedClients)
					{
						broadcastData(server, 2, event.peer -> incomingPeerID + 1, std::to_string(f));
					}

					break;
				case ENET_EVENT_TYPE_RECEIVE:
					parseData(event.packet->data);
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					std::cout << "Client " << event.peer -> incomingPeerID + 1 << " disconnected." << std::endl;
					connectedClients.erase(event.peer -> incomingPeerID + 1);
					broadcastData(server, 3, 0, std::to_string(event.peer -> incomingPeerID + 1));
				case ENET_EVENT_TYPE_NONE:
					break;
			}
		}
	}
	else if (isServer == false)
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
}

Vector2 Net::getPacket()
{
	return Vector2(x,y);
}

int Net::getId()
{
	return packetID;
}

std::vector<int> Net::allPlayers() // this is a test
{
	std::vector<int> clients(connectedClients.begin(), connectedClients.end());
	return clients;
}

void Net::sendPacket(Vector2 pos)
{
	if (isServer == true)
	{
		broadcastData(server, 1, 0, std::to_string(pos.x) + " " + std::to_string(pos.y));
	}
	else if (isServer == false)
	{
		if (CLIENT_ID != 0)
		{
			sendData(peer, 1, CLIENT_ID, std::to_string(pos.x) + " " + std::to_string(pos.y));
		}
	}
}