#pragma once
#include "components/ECS.h"

enum class LightType
{
	POINT = 1,
	SPOT = 2,
};

class LightComponent : public Component
{
public:

	LightType type;
	float intensity;
	Vec3 ambColor;
	Vec3 diffColor;
	Vec3 specColor;
	
	// Inherited via Component
	void Init(GameObject* g) override;
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;

private:
};

