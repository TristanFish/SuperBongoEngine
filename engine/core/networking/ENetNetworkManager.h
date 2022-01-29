#ifndef ENETNETWORKMANAGER_H
#define ENETNETWORKMANAGER_H

#include <mutex>
#include <vector>
#include <enet/enet.h>
#include <sstream>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

#include "NetworkManager.h"

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
	bool clientConnected = false;
	
public:

	~ENetNetworkManager();
	
	void Init() override;
	void PollNetworkEvents() override;
	void HandleNetworkEvents() override;
	
	void HandleServerEvents();
	void HandleClientEvents();
	//turns binary data into a string
	std::string ParseData(unsigned char* data) const;
	//turns string data into binary
	template<typename T>
	std::stringstream SerializeData(const T& data)
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out | std::stringstream::binary);
		//Cereal needs these open and close brackets to properly flush the Archive
		{
			cereal::BinaryOutputArchive oarchive(ss);

			//puts "data" into the stringstream "ss" as binary data
			oarchive(data);
		}
		return ss;
	}

	
	void CreateHost(unsigned int port, unsigned int maxConnections) override;
	bool Connect(const char* addressString, unsigned int port) override;
	void BroadcastPacket(const std::string& data) override;
	void SendPacket(const std::string& data) override;
	void SendPacketToPeer(const std::string& data) override;
	void Disconnect() override;
	void Cleanup() override;
};

#endif