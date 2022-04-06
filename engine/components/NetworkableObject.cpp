
#include "NetworkableObject.h"
#include "GameObject.h"
#include "core/networking/ENetNetworkManager.h"
//#include "cereal/archives/json.hpp"
#include <iostream>


NetworkableObject::NetworkableObject()
{
	isActive = false;
	networkEventType = 1;
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

	//D new stuff
	transform = gameObject->transform;
}

void NetworkableObject::Update(const float deltaTime)
{
	NetRole role = NetworkManager::GetInstance()->role;
	pos = gameObject->transform.GetPosition();

	//D
	transform = gameObject->transform;

	
	std::stringstream ss;
	{
		cereal::JSONOutputArchive oarchive(ss);
		//old call
		/*oarchive(cereal::make_nvp("NetworkType", networkEventType),
			cereal::make_nvp("ObjectName", name),
			cereal::make_nvp("position", pos));*/
		
		//short way of calling it since its already defined in .h
		NetworkableObject netobject = *this;
		netobject.serialize(oarchive);
		std::cout << ss.str() << std::endl; //for testing (temp)
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
	std::cout << ss.str() << std::endl;
	//rapidjson::Value::ConstMemberIterator itr = doc.FindMember(gameObject->name.c_str());
	//if(itr != doc.MemberEnd())	{
	//	
	//	
	//}
	std::string str = doc["ObjectName"].GetString();
	if(doc.HasMember("ObjectName"))	{
		std::string str = doc["ObjectName"].GetString();
		return std::stringstream(str);
	}
	
	return std::stringstream("NoNameFound");
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
			case 1:	{
				float posx = doc["posx"].GetFloat();
				float posy = doc["posy"].GetFloat();
				float posz = doc["posz"].GetFloat();
					
				float rotx = doc["rotx"].GetFloat();
				float roty = doc["roty"].GetFloat();
				float rotz = doc["rotz"].GetFloat();
				float rotw = doc["rotw"].GetFloat();
					
				float scalex = doc["scalex"].GetFloat();
				float scaley = doc["scaley"].GetFloat();
				float scalez = doc["scalez"].GetFloat();

				pos = MATH::Vec3(posx, posy, posz);
				gameObject->SetPos(pos);
					
				rot = MATH::Quaternion(MATH::Vec4(rotx, roty, rotz, rotw));
				gameObject->SetRotation(rot);

				scale = MATH::Vec3(scalex, scaley, scalez);
				gameObject->SetScale(scale);
				
				break;
			}
			  default:
				  break;
			
		}
	//gameObject->SetPos(tmp);

	
}



