#pragma once

#include "components/ECS.h"
#include "components/Components.h"
class Grass : public GameObject, public MeshRenderer
{
public:
	Grass();
	Grass(const char* name, MATH::Vec3 position);
	~Grass();

	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;

private:
	unsigned int amount;
	MATH::Matrix4* modelMatricies;


	void CalculateModelMatrices();
};

