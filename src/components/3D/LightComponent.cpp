#include "LightComponent.h"

using namespace MATH;

void LightComponent::Init(GameObject* g)
{
	gameobject = g;
	type = LightType::POINT;
	ambColor = Vec3(0.1f, 0.0f, 0.0f);
	diffColor = Vec3(0.0f, 1.0f, 0.0f);
	specColor = Vec3(0,0,1);
}

const char* LightComponent::ComponentName() const
{
	return "LightComponent";
}
