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
	//lightCube.DrawCube(gameobject->transform.GetPosition(), Vec3(5.0f, 5.0f, 5.0f), false, Vec3(1.0f, 0.0f, 0.0f));
}

void LightComponent::HandleEvents(const SDL_Event& event)
{
}
