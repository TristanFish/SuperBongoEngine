#include "NetworkableObject.h"
#include "GameObject.h"
#include "core/networking/ENetNetworkManager.h"
#include "cereal/archives/json.hpp"
#include <iostream>


NetworkableObject::NetworkableObject()
{
	isActive = false;
	networkEventType = (int)NetworkEventTypes::POSITIONUPDATE;
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
	pos = gameObject->transform.GetPosition();
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		oarchive(cereal::make_nvp("NetworkType", networkEventType), cereal::make_nvp("ObjectName", name), cereal::make_nvp("posx", pos.x), cereal::make_nvp("posy", pos.y), cereal::make_nvp("posz", pos.z));
		
	}

	if (role == NetRole::SERVER) {
		NetworkManager::GetInstance()->SendPreserializedPacket(ss);
	} 	

}


std::stringstream NetworkableObject::FindGameObjectName(std::string& s)
{
	std::stringstream ss(s);
	cereal::JSONInputArchive is(ss);
	rapidjson::Document doc;
	doc.Parse(ss.str().c_str());
	std::string str = doc["ObjectName"].GetString();
	return std::stringstream(str);
}

void NetworkableObject::RecievePositionData(std::stringstream& ss)
{

		rapidjson::Document doc;
		int netType;
		{
			std::string  string = ss.str();
			std::stringstream os(string);
			cereal::JSONInputArchive iarchive(os);
			//Find network message type
			doc.Parse(string.c_str());
			if (doc["NetworkType"].IsNumber()) {
				netType = doc["NetworkType"].GetInt();
			}
		}
		switch(netType) {
			case 1:
				//Position/transform update
				float posx = doc["posx"].GetFloat();
				float posy = doc["posy"].GetFloat();
				float posz = doc["posz"].GetFloat();

				pos.x = posx;
				pos.y = posy;
				pos.z = posz;
				gameObject->SetPos(pos);
			break;
		}
	//gameObject->SetPos(tmp);

	
}



