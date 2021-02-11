#ifndef SCENE2_H
#define SCENE2_H

#include "scenes/Scene.h"

class Scene2 : public Scene
{
public:
	Scene2();
	~Scene2();
	// Inherited via Scene
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Reset() override;
	virtual void Update(float deltaTime_) override;
	virtual void HandleEvents(const SDL_Event& event) override;
	virtual void Render() const override;
};

#endif