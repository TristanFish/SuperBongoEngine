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

class NetworkManager
{
public:

	NetRole role;

protected:
	std::thread networkPollingThread;
	static std::unique_ptr<NetworkManager> networkInstance;
	friend std::default_delete<NetworkManager>;
	bool initialized;

public:
	
	static NetworkManager* GetInstance();
	
	virtual ~NetworkManager() = default;
	
	virtual void Init() = 0;
	
	virtual void PollNetworkEvents() = 0;

	virtual void HandleNetworkEvents() = 0;

	virtual void CreateHost(unsigned int port, unsigned int maxConnections) = 0;

	virtual bool Connect(const char* addressString, unsigned int port) = 0;

	virtual void BroadcastPacket(const std::string& data) = 0;
	
	virtual void SendPacket(const std::string& data) = 0;

	virtual void SendPacketToPeer(const std::string& data) = 0;

	virtual void Disconnect() = 0;
	
	virtual void Cleanup() = 0;

	
};


#endif
