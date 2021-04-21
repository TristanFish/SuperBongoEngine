#pragma once
#include "components/ECS.h"
#include "components/3D/LightComponent.h"


class LightObject : public GameObject, public LightComponent
{
public:
	Debug light;

	LightObject(const char* name, MATH::Vec3 position);
	~LightObject() override = default;
	// Inherited via GameObject
	void DrawDebugGeometry() const override;
};