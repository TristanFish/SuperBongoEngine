#include "NetworkableObject.h"
#include "GameObject.h"
#include "core/networking/ENetNetworkManager.h"
#include "cereal/archives/json.hpp"
#include <iostream>

NetworkableObject::NetworkableObject()
{
	isActive = false;
}

NetworkableObject::~NetworkableObject()
{
}

void NetworkableObject::Init(GameObject* g)
{
	isActive = false;
	gameObject = g;
	pos = gameObject->transform.GetPosition();
	name = gameObject->GetName();
}

void NetworkableObject::Update(const float deltaTime)
{
	NetRole role = NetworkManager::GetInstance()->role;


	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(cereal::make_nvp("ObjectName", name), cereal::make_nvp("posx", pos.x), cereal::make_nvp("posy", pos.y), cereal::make_nvp("posz", pos.z));
		NetworkManager::GetInstance();
	}
	if (role == NetRole::SERVER) {
		NetworkManager::GetInstance()->SendPreserializedPacket(ss);
	}

	
}


void NetworkableObject::RecievePositionData(std::stringstream ss)
{
		{
			std::string  string = ss.str();
			std::stringstream os(string);
			cereal::JSONInputArchive iarchive(os);
			std::cout << os.str() << std::endl;
		}
	//gameObject->SetPos(tmp);

	
}



