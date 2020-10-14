#pragma once

#include "components/ECS.h"
#include "components/Components.h"
#include "custom/Instancer.h"
class Grass : public GameObject, public MeshRenderer, Instancer
{
public:
	Grass();
	Grass(const char* name, MATH::Vec3 position, unsigned int amount_);
	~Grass();

	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;

private:

};

