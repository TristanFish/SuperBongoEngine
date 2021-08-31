#pragma once
#include "components/ECS.h"
#include "components/3D/LightComponent.h"


class LightObject : public GameObject, public LightComponent
{
public:
	Debug light;

	LightObject(std::string name, MATH::Vec3 position);
	~LightObject() override = default;
	// Inherited via GameObject
	void DrawDebugGeometry() const override;

	LightObject* GetClone() const override { return new LightObject(this->name, this->transform.pos); };

};