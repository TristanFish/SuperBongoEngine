#ifndef ENETNETWORKMANAGER_H
#define ENETNETWORKMANAGER_H

#include <mutex>
#include <vector>
#include <enet/enet.h>
#include "components/SceneGraph.h"
#include "NetworkManager.h"

class SceneGraph;

enum class NetworkEventTypes : int
{
	POSITIONUPDATE = 1,
	MESSAGE
};
class TempSemaphore
{
	std::mutex mutex;
	std::condition_variable condition;
	short count = 0;

public:
	TempSemaphore(short count_)
	{
		count = count_;
	}

	void Release()
	{
		std::lock_guard<decltype(mutex)> lock(mutex);
		count++;
		condition.notify_one();
	}

	void Acquire()
	{
		std::unique_lock<decltype(mutex)> lock(mutex);
		while(!count)
		{
			condition.wait(lock);
		}
		count--;
	}

	bool TryAcquire()
	{
		std::lock_guard<decltype(mutex)> lock(mutex);
		if(count)
		{
			count--;
			return true;
		}
		return false;
	}
};

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
	//
	std::string ParseJsonData(unsigned char* data) const;
	//turns string data into binary
	std::stringstream SerializeData(const std::string& data);
	
	void CreateHost(unsigned int port, unsigned int maxConnections) override;
	bool Connect(const char* addressString, unsigned int port) override;
	void BroadcastPacket(const std::string& data) override;
	void SendPacket(const std::string& data) override;
	void SendPreserializedPacket(std::stringstream& ss) override;
	void SendPacketToPeer(const std::string& data) override;
	void Disconnect() override;
	void Cleanup() override;
};

#endif