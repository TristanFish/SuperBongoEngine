#include "ENetNetworkManager.h"
#include "core/Logger.h"

#include <sstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

using namespace std;

void ENetNetworkManager::Init()
{
	if(enet_initialize() != 0)
	{
		EngineLogger::Warning("ENet failed to initialize", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	}

	atexit(enet_deinitialize);
	EngineLogger::Info("ENet initialized successfully", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	initialized = true;
}

void ENetNetworkManager::PollNetworkEvents()
{
	EngineLogger::Info("Started polling for network events", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	do
	{
		while(enet_host_service(user, &netEvent, 0) > 0)
		{
			HandleNetworkEvents();
		}
	}
	while(initialized);
}

void ENetNetworkManager::HandleNetworkEvents()
{
	if(role == NetRole::SERVER)
	{
		HandleServerEvents();
	} else if(role == NetRole::CLIENT)
	{
		HandleClientEvents();
	}
}

void ENetNetworkManager::HandleServerEvents()
{
	switch (netEvent.type)
	{
		case ENET_EVENT_TYPE_NONE:
			{


				
				break;
			}
		case ENET_EVENT_TYPE_CONNECT:
			{
				peerCount++;
				connectedPeers.emplace_back(netEvent.peer);
				EngineLogger::Info("A new client has connected, " + to_string(peerCount) + " peers connected to the server", 
								  "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
				break;
			}
		case ENET_EVENT_TYPE_DISCONNECT:
			{
				peerCount--;
				EngineLogger::Info("A client has disconnected, " + to_string(peerCount) + " peers connected to the server", 
								  "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
				break;
			}
		case ENET_EVENT_TYPE_RECEIVE:
			{
				string parsedData = ParseData(netEvent.packet->data);
				EngineLogger::Info("Received packet containing \"" + parsedData + "\"", 
								  "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
				break;
			}
		default: ;
	}
}

void ENetNetworkManager::HandleClientEvents()
{
	switch (netEvent.type)
	{
		case ENET_EVENT_TYPE_NONE:
			{

				
				break;
			}
		case ENET_EVENT_TYPE_CONNECT:
			{
				EngineLogger::Info("A peer has connected to the client", 
								  "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
				break;
			}
		case ENET_EVENT_TYPE_DISCONNECT:
			{
				EngineLogger::Info("Client recieved disconnect event", 
								  "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
				break;
			}
		case ENET_EVENT_TYPE_RECEIVE:
			{
				string parsedData = ParseData(netEvent.packet->data);
				EngineLogger::Info("Received packet containing \"" + parsedData + "\"", 
								  "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
				break;
			}
		default: ;
	}
}

std::string ENetNetworkManager::ParseData(unsigned char* data) const
{
	char buffer[DEFAULT_BUFFER_LENGTH];
	memcpy(buffer, data, netEvent.packet->dataLength);
	stringstream ss(string(buffer, netEvent.packet->dataLength), stringstream::in | stringstream::out | stringstream::binary);
	string deserializedData;
	{
		cereal::BinaryInputArchive iArchive(ss);

		iArchive(deserializedData);
	}

	return deserializedData;
}

stringstream ENetNetworkManager::SerializeData(const string& data)
{
	stringstream ss(stringstream::in | stringstream::out | stringstream::binary);
	{
		cereal::BinaryOutputArchive oarchive(ss);

		oarchive(data);
	}

	return ss;
}

void ENetNetworkManager::CreateHost(unsigned int port, unsigned int maxConnections)
{
	if(role == NetRole::SERVER)
	{
		address.port = port;
		address.host = ENET_HOST_ANY;
		user = enet_host_create(&address, maxConnections, 1, 0, 0);
	}
	else if(role == NetRole::CLIENT)
	{
		user = enet_host_create(nullptr, 1, 1, 0, 0);
		address.port = port;
		enet_address_set_host(&address, "127.0.0.1");
	}


	if(user == nullptr)
	{
		EngineLogger::Info("Failed to create host", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	}
	EngineLogger::Info("Host successfully created", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	if(role == NetRole::SERVER)
	{
		networkPollingThread = thread(&ENetNetworkManager::PollNetworkEvents, this);
		networkPollingThread.detach();
	}
}

bool ENetNetworkManager::Connect(const char* addressString, unsigned int port)
{
	if(role == NetRole::CLIENT)
	{
		address.port = port;
		enet_address_set_host(&address, "127.0.0.1");
		ENetPeer* peer = enet_host_connect(user, &address, 1, 0);
		if(peer == nullptr)
		{
			EngineLogger::Info("No available peers to connect to", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
			return false;
		}
		
		EngineLogger::Info("Peer available to connect to", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);

		ENetEvent connectEvent;
		if(enet_host_service(user, &connectEvent, 5000) > 0 && connectEvent.type == ENET_EVENT_TYPE_CONNECT)
		{
			EngineLogger::Info("Successfully connected to peer", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
			connectedPeers.emplace_back(peer);
			networkPollingThread = thread(&ENetNetworkManager::PollNetworkEvents, this);
			networkPollingThread.detach();
			return true;
		}
		else
		{
			enet_peer_reset(peer);
			EngineLogger::Info("Connection to peer failed", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
			return false;
		}
		
	}
	else
	{
		EngineLogger::Info("Connect function is for clients only", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
		return false;
	}
}

void ENetNetworkManager::BroadcastPacket(const string& data)
{
	char buffer[DEFAULT_BUFFER_LENGTH];

	stringstream ss = SerializeData(data);
	int size = ss.rdbuf()->str().size();
	//copy stringstream into char buffer
	ss.read(buffer, size);

	ENetPacket* packet = enet_packet_create(buffer, size, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(user, 0, packet);

	EngineLogger::Info("Packet containing \"" + ss.rdbuf()->str() + "\" was sent to all clients", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
}

void ENetNetworkManager::SendPacket(const string& data)
{
	char buffer[DEFAULT_BUFFER_LENGTH];

	stringstream ss = SerializeData(data);
	int size = ss.rdbuf()->str().size();
	//copy stringstream into char buffer
	ss.read(buffer, size);

	ENetPacket* packet = enet_packet_create(buffer, size, ENET_PACKET_FLAG_RELIABLE);
	if(peerCount > 0)
	{
		enet_peer_send(connectedPeers[0], 0, packet);
		EngineLogger::Info("Packet containing \"" + ss.rdbuf()->str() + "\" was sent to the first peer", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	}

}

void ENetNetworkManager::SendPacketToPeer(const string& data)
{
}

void ENetNetworkManager::Disconnect()
{
	if(role == NetRole::CLIENT)
	{
		enet_peer_disconnect(connectedPeers[0], 0);
		enet_host_destroy(user);
	}
	else if(role == NetRole::SERVER)
	{
		enet_host_destroy(user);
	}
}

void ENetNetworkManager::Cleanup()
{
	initialized = false;
}
