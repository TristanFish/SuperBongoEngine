#include "ENetNetworkManager.h"
#include "core/Logger.h"
#include"../Globals.h"

#include <sstream>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>

using namespace std;

ENetNetworkManager::~ENetNetworkManager()
{
	//if we are a client and still connected to a peer, disconnect ourselves
	if(role == NetRole::CLIENT && clientConnected)
	{
		ENetNetworkManager::Disconnect();
	}
	
	initialized = false;
	//if we are a server stop polling for events
	if(role == NetRole::SERVER)
	{
		networkPollingThread.join();
	}

	//destroy our host
	if(user != nullptr)
	{
		enet_host_destroy(user);
		user = nullptr;
	}
	//deinitialize enet
	enet_deinitialize();
}

void ENetNetworkManager::Init()
{
	if(enet_initialize() != 0)
	{
		EngineLogger::Warning("ENet failed to initialize", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	}

	EngineLogger::Info("ENet initialized successfully", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	//connectedPeers = std::vector<ENetPeer*>();
	connectedPeers.reserve(2);
	initialized = true;
}

void ENetNetworkManager::PollNetworkEvents()
{
	EngineLogger::Info("Started polling for network events", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	//if we are a client continue polling for events until we're not connected to anyone
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
	}//if we are a server continue polling for events until we start shutting down enet
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
}

//Handle incoming server events based on what type of event they are
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
				//Receive is gonna need to be much more fleshed out
				//Needs to track which client it's receiving from
				//Needs to know what kind of data it's receiving
				string parsedData = ParseJsonData(netEvent.packet->data);
				EngineLogger::Info("Received packet containing \"" + parsedData + "\"", 
								  "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
				Globals::Engine::GetSceneGraph()->GameObjectNetworkUpdate(parsedData);
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
				//string parsedData = ParseData(netEvent.packet->data);
				
				string parsedData = ParseJsonData(netEvent.packet->data);
				EngineLogger::Info("Received packet containing \"" + parsedData + "\"", 
								  "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
				Globals::Engine::s_SceneGraph->GameObjectNetworkUpdate(parsedData);
				break;
			}
		default: ;
	}
}

std::string ENetNetworkManager::ParseData(unsigned char* data) const
{
	//Copy data into this buffer so it can be turned from binary back into a string
	char buffer[DEFAULT_BUFFER_LENGTH];
	memcpy(buffer, data, netEvent.packet->dataLength);
	stringstream ss(string(buffer, netEvent.packet->dataLength), stringstream::in | stringstream::out | stringstream::binary);
	string deserializedData;
	//Cereal needs these open and close brackets to properly flush the Archive
	{
		cereal::BinaryInputArchive iArchive(ss);

		iArchive(deserializedData);
	}

	return deserializedData;
}

std::string ENetNetworkManager::ParseJsonData(unsigned char* data) const
{
	//Copy data into this buffer so it can be turned from binary back into a string
	char buffer[DEFAULT_BUFFER_LENGTH];
	memcpy(buffer, data, netEvent.packet->dataLength);
	stringstream ss(string(buffer, netEvent.packet->dataLength), stringstream::in | stringstream::out | stringstream::binary);
	string deserializedData;
	//Cereal needs these open and close brackets to properly flush the Archive
	{
		cereal::JSONInputArchive iArchive(ss);

		//iArchive(deserializedData);
	}

	return ss.str();
}

stringstream ENetNetworkManager::SerializeData(const string& data)	{
	stringstream ss(stringstream::in | stringstream::out | stringstream::binary);
	//Cereal needs these open and close brackets to properly flush the Archive
	{
		cereal::BinaryOutputArchive oarchive(ss);

		//puts "data" into the stringstream "ss" as binary data
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
		//if we're a server start polling as soon as we're created
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
		//Check for a connect event
		if(enet_host_service(user, &connectEvent, 1000) > 0 && connectEvent.type == ENET_EVENT_TYPE_CONNECT)
		{
			EngineLogger::Info("Successfully connected to peer", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
			connectedPeers.emplace_back(peer);
			clientConnected = true;
			//if we're a client start polling as soon as we've connected to a server
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

	EngineLogger::Info("Packet containing \"" + data + "\" was sent to all clients", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
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
		//At the moment only sends data to the first connected peer
		enet_peer_send(connectedPeers[0], 0, packet);
		EngineLogger::Info("Packet containing \"" + data + "\" was sent to the first peer", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	}
	else
	{
		EngineLogger::Info("There are no connected peers to send a message to", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	}

}

void ENetNetworkManager::SendPreserializedPacket(std::stringstream& ss)	{
	char buffer[DEFAULT_BUFFER_LENGTH];
	int size = ss.rdbuf()->str().size();
	//copy stringstream into char buffer
	ss.read(buffer, size);

	ENetPacket* packet = enet_packet_create(buffer, size, ENET_PACKET_FLAG_RELIABLE);
	
	if (!connectedPeers.empty())
	{
		//At the moment only sends data to the first connected peer
		enet_peer_send(connectedPeers[0], 0, packet);
		EngineLogger::Info("Packet containing \"" + ss.str() + "\" was sent to the first peer", "ENetNetworkManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
	}
	else
	{
		EngineLogger::Info("There are no connected peers to send a message to", "ENetNetworkingManager.cpp", __LINE__, MessageTag::TYPE_NETWORK);
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
			clientConnected = false;
			//since we're no longer connected to a server stop polling for events
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
