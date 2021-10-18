#ifndef ENETNETWORKMANAGER_H
#define ENETNETWORKMANAGER_H

#include <vector>

#include "NetworkManager.h"
#include <enet/enet.h>

constexpr unsigned int DEFAULT_BUFFER_LENGTH = 512;

//At the moment there are a lot of hard coded values in the cpp of this class
//because its only tested for connecting to yourself
//Dont be surprised if some parameter values dont modify the actual code
class ENetNetworkManager :public NetworkManager
{
private:
	ENetAddress address;
	ENetHost* user;
	ENetEvent netEvent;
	std::vector<ENetPeer*> connectedPeers;
	unsigned int peerCount = 0;
	
public:
	void Init() override;
	void PollNetworkEvents() override;
	void HandleNetworkEvents() override;
	
	void HandleServerEvents();
	void HandleClientEvents();
	std::string ParseData(unsigned char* data) const;
	std::stringstream SerializeData(const std::string& data);
	
	void CreateHost(unsigned int port, unsigned int maxConnections) override;
	bool Connect(const char* addressString, unsigned int port) override;
	void BroadcastPacket(const std::string& data) override;
	void SendPacket(const std::string& data) override;
	void SendPacketToPeer(const std::string& data) override;
	void Disconnect() override;
	void Cleanup() override;
};

#endif