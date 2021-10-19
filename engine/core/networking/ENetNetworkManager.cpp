#include "ENetNetworkManager.h"
#include "core/Logger.h"

#include <sstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

using namespace std;

ENetNetworkManager::~ENetNetworkManager()
{
	initialized = false;
	if(role == NetRole::SERVER)
	{
		networkPollingThread.join();
	}
	
	if(user != nullptr)
	{
		enet_host_destroy(user);
		user = nullptr;
	}
	enet_deinitialize();
}

void ENetNetworkManager::Init()
{
	if(enet_initialize() != 0)
	{
		EngineLogger::Warning("ENet failed to initialize", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	}

	EngineLogger::Info("ENet initialized successfully", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	initialized = true;
}

void ENetNetworkManager::PollNetworkEvents()
{
	EngineLogger::Info("Started polling for network events", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	if(role == NetRole::CLIENT)
	{
		do
		{
			while(enet_host_service(user, &netEvent, 1000) > 0)
			{
				HandleClientEvents();
			}
		}
		while(clientConnected);
	}
	else if(role == NetRole::SERVER)
	{
		do
		{
			while(enet_host_service(user, &netEvent, 1000) > 0)
			{
				HandleServerEvents();
			}
		}
		while(initialized);
	}

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
				connectedPeers.emplace_back(netEvent.peer);
				EngineLogger::Info("A new client has connected, " + to_string(connectedPeers.size()) + " peers connected to the server", 
								  "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
				break;
			}
		case ENET_EVENT_TYPE_DISCONNECT:
			{
				connectedPeers.erase(find(connectedPeers.begin(), connectedPeers.end(), netEvent.peer));
				EngineLogger::Info("A client has disconnected, " + to_string(connectedPeers.size()) + " peers connected to the server", 
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
		if(enet_host_service(user, &connectEvent, 1000) > 0 && connectEvent.type == ENET_EVENT_TYPE_CONNECT)
		{
			EngineLogger::Info("Successfully connected to peer", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
			connectedPeers.emplace_back(peer);
			//Start polling for events
			networkPollingThread = thread(&ENetNetworkManager::PollNetworkEvents, this);
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
	if(!connectedPeers.empty())
	{
		enet_peer_send(connectedPeers[0], 0, packet);
		EngineLogger::Info("Packet containing \"" + ss.str() + "\" was sent to the first peer", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	}

}

void ENetNetworkManager::SendPacketToPeer(const string& data)
{
}

void ENetNetworkManager::Disconnect()
{
	if(!connectedPeers.empty())
	{
		if(role == NetRole::CLIENT)
		{
			networkPollingThread.join();
			enet_peer_disconnect(connectedPeers[0], 0);

			while(enet_host_service(user, &netEvent, 3000) > 0)
			{
				if(netEvent.type == ENET_EVENT_TYPE_DISCONNECT)
				{
					EngineLogger::Info("Disconnect successful", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
				}
			}
		}
		else if(role == NetRole::SERVER)
		{

		}
		
		connectedPeers.clear();
	}
}

void ENetNetworkManager::Cleanup()
{

}
