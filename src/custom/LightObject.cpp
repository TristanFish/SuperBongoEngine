#include "LightObject.h"

LightObject::LightObject()
{
}

LightObject::LightObject(const char* name, Vec3 position)
{
	this->name = name;
	transform.pos = position;

	typeID = Type::Light;


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

}

void LightObject::HandleEvents(const SDL_Event& event)
{
	LightComponent::HandleEvents(event);
}

void LightObject::DrawDebugGeometry()
{
	light.DrawCube(transform.pos, Vec3(2.0f), false, Vec4(1.0f, 1.0f, 0.0f, 1.0f));
}

