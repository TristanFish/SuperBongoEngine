#include "LightComponent.h"

using namespace MATH;

void LightComponent::Init(GameObject* g)
{
	gameobject = g;
	type = LightType::POINT;
	ambColor = Vec3(0.1f, 0.0f, 0.0f);
	diffColor = Vec3(0.0f, 1.0f, 0.0f);
	specColor = Vec3(0,0,1);
	intensity = 1.0f;
}

void LightComponent::OnSaveComponent(const std::string& saveName, std::string parentName)
{

}
