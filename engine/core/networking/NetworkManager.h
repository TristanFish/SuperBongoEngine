#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include <memory>
#include <string>
#include <thread>


enum class NetRole
{
	SERVER = 1,
	CLIENT
};

//! Network Manager Interface
/*! Is a blueprint for creating any network managers */
class NetworkManager
{
public:
	//The user's current network role Server or client
	NetRole role;

protected:
	//Thread that's used to poll for network events
	std::thread networkPollingThread;
	//Network Manager's a singleton
	static std::unique_ptr<NetworkManager> networkInstance;
	//Whether Online functionality is initialized or not
	bool initialized;

public:
	
	static NetworkManager* GetInstance();
	
	virtual ~NetworkManager() = default;

	//Initializes the networking system
	virtual void Init() = 0;
	//Checks for any incoming network events
	virtual void PollNetworkEvents() = 0;
	//Determines how incoming events are handled
	virtual void HandleNetworkEvents() = 0;
	//Creates a representation of the user's network info
	virtual void CreateHost(unsigned int port, unsigned int maxConnections) = 0;
	//Used for connecting clients to other devices
	virtual bool Connect(const char* addressString, unsigned int port) = 0;
	//Used for servers to send packets to all connected clients
	virtual void BroadcastPacket(const std::string& data) = 0;
	//Send a packet to the first available peer
	virtual void SendPacket(const std::string& data) = 0;

	virtual void SendPreserializedPacket(std::stringstream& ss) = 0;

	virtual void SendPacketToPeer(const std::string& data) = 0;
	//Disconnects the client if it is connected to a server
	virtual void Disconnect() = 0;
	
	virtual void Cleanup() = 0;

	
};


#endif
