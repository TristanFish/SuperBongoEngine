#ifndef NETWORKABLEOBJECT_H
#define NETWORKABLEOBJECT_H

#include "Component.h"
#include <string>
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

	bool isActive;

	NetworkableObject();
	~NetworkableObject();

	void Init(GameObject* g) override;
	void Update(const float deltaTime) override;
	void Render() const override {}
	void HandleEvents(const SDL_Event& event) override {}

	std::stringstream FindGameObjectName(std::string& s);
	virtual void RecievePositionData(std::stringstream& ss);

	template<class Archive>
	void serialize(Archive& ar)	{
		ar(cereal::make_nvp("NetworkType", networkEventType), 
			cereal::make_nvp("ObjectName", name), 
			cereal::make_nvp("posx", transform.pos.x),
			cereal::make_nvp("posy", transform.pos.y),
			cereal::make_nvp("posz", transform.pos.z),
			cereal::make_nvp("rotx", transform.rotation.GetQuat().x),
			cereal::make_nvp("roty", transform.rotation.GetQuat().y),
			cereal::make_nvp("rotz", transform.rotation.GetQuat().z),
			cereal::make_nvp("rotw", transform.rotation.GetQuat().w),
			cereal::make_nvp("scalex", transform.scale.x),
			cereal::make_nvp("scaley", transform.scale.y),
			cereal::make_nvp("scalez", transform.scale.z)		
			);
	}

	
private:
	
	
};
#endif

