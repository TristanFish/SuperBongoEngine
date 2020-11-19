#include "LightComponent.h"
#include "core/Debug.h"

void LightComponent::Init(GameObject* g)
{
	gameobject = g;
	type = LightType::POINT;
	ambColor = Vec3(0.1f, 0.0f, 0.0f);
	diffColor = Vec3(0.0f, 1.0f, 0.0f);
	specColor = Vec3(0,0,1);
}

void LightComponent::Update(const float deltaTime)
{
}

void LightComponent::Render() const
{
}

void LightComponent::HandleEvents(const SDL_Event& event)
{
}
