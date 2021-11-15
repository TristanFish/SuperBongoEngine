#include "NetworkableObject.h"
#include "GameObject.h"
#include "core/networking/ENetNetworkManager.h"
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
	std::stringstream posData;

	posData << pos.x << ',';
	posData << pos.y << ',';
	posData << pos.z;

	std::string str = posData.str();
	std::stringstream ss;
	{
		cereal::BinaryOutputArchive oarchive(ss);

		oarchive(str, name);		
		NetworkManager::GetInstance();
	}

	if (role == NetRole::SERVER) {
		NetworkManager::GetInstance()->SendPreserializedPacket(ss);
	}

	
}


void NetworkableObject::RecievePositionData(std::stringstream ss)
{
	MATH::Vec3 tmp;
	std::string name;
	{
		cereal::BinaryInputArchive iarchive(ss);
		iarchive(name, tmp.x, tmp.y, tmp.z);
		std::cout << name <<tmp.x << tmp.y << tmp.z << std::endl;
	}
	gameObject->SetPos(tmp);

	
}



