#pragma once
#include "components/ECS.h"
#include "components/3D/LightComponent.h"


class LightObject : public GameObject, public LightComponent
{
public:

	LightObject(const std::string& name, MATH::Vec3 position);
	~LightObject() override = default;
	// Inherited via GameObject
	void DrawDebugGeometry() const override;

	std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<LightObject>(this->name, this->transform.GetPosition()); }

};