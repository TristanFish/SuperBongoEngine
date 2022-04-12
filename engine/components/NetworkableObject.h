#ifndef NETWORKABLEOBJECT_H
#define NETWORKABLEOBJECT_H

#include "Component.h"

#include <cereal/archives/json.hpp> //D changed this from binary.hpp to json since we use json, code ran like before change
#include <cereal/types/string.hpp>
#include <cereal/cereal.hpp>

#include "VMath.h"
#include "Transform.h"

class NetworkableObject : public Component	{
public:

	MATH::Vec3 pos, scale;
	MATH::Quaternion rot;

	Transform transform;
	
	
	
	std::string name, position;
	int networkEventType;


	NetworkableObject();
	~NetworkableObject();

	void Init(std::shared_ptr<GameObject> g) override;
	void Update(const float deltaTime) override;
	void HandleEvents(const SDL_Event& event) override {}

	std::stringstream FindGameObjectName(std::string& s);
	virtual void RecievePositionData(std::stringstream& ss);

	template<class Archive>
	void serialize(Archive& ar)	{
		ar(cereal::make_nvp("NetworkType", networkEventType), 
			cereal::make_nvp("ObjectName", name), 
			cereal::make_nvp("posx", transform.GetPosition().x),
			cereal::make_nvp("posy", transform.GetPosition().y),
			cereal::make_nvp("posz", transform.GetPosition().z),
			cereal::make_nvp("rotx", transform.GetRotationQuat().GetQuat().x),
			cereal::make_nvp("roty", transform.GetRotationQuat().GetQuat().y),
			cereal::make_nvp("rotz", transform.GetRotationQuat().GetQuat().z),
			cereal::make_nvp("rotw", transform.GetRotationQuat().GetQuat().w),
			cereal::make_nvp("scalex", transform.GetScale().x),
			cereal::make_nvp("scaley", transform.GetScale().y),
			cereal::make_nvp("scalez", transform.GetScale().z)		
			);
	}

	
private:
	
	
};
#endif

