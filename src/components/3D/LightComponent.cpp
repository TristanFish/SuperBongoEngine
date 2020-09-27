#include "LightComponent.h"
#include "core/Debug.h"

void LightComponent::Init(GameObject* g)
{
	gameobject = g;
	type = LightType::POINT;
	ambColor = Vec3(0.9f, 0.0f, 0.0f);
	diffColor = Vec3(0.5f, 0.0f, 0.5f);
	specColor = Vec3(0,0,1);
}

void LightComponent::Update(const float deltaTime)
{
}

void LightComponent::Render() const
{
	//Debug::DrawCube(gameobject->transform.GetPosition(), Vec3(55.0f), true, Vec4(1.0f, 0.0f, 0.0f, 0.0f));
}

void LightComponent::HandleEvents(const SDL_Event& event)
{
}
