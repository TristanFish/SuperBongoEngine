#ifndef SCENE2_H
#define SCENE2_H

#include "core/scene/Scene.h"

class Scene2 final : public Scene
{
public:
	Scene2();
	
	// Inherited via Scene
	bool OnCreate() override;
	void OnDestroy() override {}
	void Update(float deltaTime_) override;
	void HandleEvents(const SDL_Event& event) override;
	void Render() override;
};

#endif