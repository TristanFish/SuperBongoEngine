#pragma once

#include "components/Components.h"

class Water : public GameObject, public MeshRenderer
{
private:
	
	Water(const char* name, MATH::Vec3 pos);
	~Water();

	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;


};

