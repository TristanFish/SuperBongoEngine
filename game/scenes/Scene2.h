#ifndef SCENE2_H
#define SCENE2_H

#include "scenes/Scene.h"

class Scene2 final : public Scene
{
public:
	Scene2() = default;
	
	// Inherited via Scene
	bool OnCreate() override;
	void OnDestroy() override {}
	void Reset() override;
	void Update(float deltaTime_) override;
	void HandleEvents(const SDL_Event& event) override;
	void Render() const override;
};

#endif