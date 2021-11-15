#ifndef NETWORKABLEOBJECT_H
#define NETWORKABLEOBJECT_H

#include "Component.h"
#include <string>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/cereal.hpp>
#include "VMath.h"


class NetworkableObject : public Component
{
public:

	MATH::Vec3 pos, scale, rot;
	std::string name, position;

	bool isActive;

	NetworkableObject();
	~NetworkableObject();

	void Init(GameObject* g) override;
	void Update(const float deltaTime) override;
	void Render() const override {}
	void HandleEvents(const SDL_Event& event) override {}

	virtual void RecievePositionData(std::stringstream ss);

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(name, position);
	}

	
private:
	
};
#endif

