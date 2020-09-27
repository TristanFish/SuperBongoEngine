#ifndef SCENE1_H
#define SCENE1_H
#include "Scene.h"
#include "custom/Camera.h"
#include "custom/TestModel.h"
#include "custom/Plane.h"
#include "graphics/Shader.h"
#include "custom/LightObject.h"

class Player;

class Scene1 : public Scene
{
private:

	Player* player;
	TestModel* ball;
	Plane* plane;
	LightObject* light;

public:
	Scene1();
	~Scene1();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &event) override;
	virtual void Reset() override;

};

#endif