#include "core/networking/NetworkManager.h"
#include "core/networking/ENetNetworkManager.h"


std::unique_ptr<NetworkManager> NetworkManager::networkInstance = nullptr;

NetworkManager* NetworkManager::GetInstance()
{
	if(networkInstance == nullptr)
	{
		networkInstance.reset(new ENetNetworkManager());
	}
	return networkInstance.get();
}
