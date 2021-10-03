#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "components/ECS.h"
#include "core/Debug.h"

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
	MATH::Vec3 ambColor;
	MATH::Vec3 diffColor;
	MATH::Vec3 specColor;
	
	// Inherited via Component
	void Init(GameObject* g) override;
	void Update(const float deltaTime) override {}
	void Render() const override {}
	void HandleEvents(const SDL_Event& event) override {}


	void OnSaveComponent(const std::string& saveName, std::string parentName) override;
private:

	Debug lightCube;
};
#endif
