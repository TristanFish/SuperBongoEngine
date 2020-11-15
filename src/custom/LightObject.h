#pragma once
#include "components/ECS.h"
#include "components/3D/LightComponent.h"


class LightObject : public GameObject, public LightComponent
{
public:
	Debug light;

	LightObject();
	LightObject(const char* name, Vec3 position);
	~LightObject();
	// Inherited via GameObject
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;
	virtual void DrawDebugGeometry() override;
};