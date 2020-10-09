#include "LightObject.h"

LightObject::LightObject()
{
}

LightObject::LightObject(const char* name, Vec3 position)
{
	this->name = name;
	transform.pos = position;

	LightComponent::Init(this);
}

LightObject::~LightObject()
{
}

void LightObject::Update(const float deltaTime)
{
	LightComponent::Update(deltaTime);
}

void LightObject::Render() const
{
	LightComponent::Render();
}

void LightObject::HandleEvents(const SDL_Event& event)
{
	LightComponent::HandleEvents(event);
}
