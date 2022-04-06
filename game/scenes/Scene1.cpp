#include "Scene1.h"
#include "core/MouseRay.h"
#include "core/Logger.h"
#include "core/Globals.h"
#include "gameObjects/LightObject.h"


Scene1::Scene1()
{
	Scene_Name = "Scene1";
}

bool Scene1::OnCreate()
{
	return Scene::OnCreate();
}

void Scene1::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
}

void Scene1::Render() 
{
	Scene::Render();
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	Scene::HandleEvents(event);
}

void Scene1::Reset()
{
	OnDestroy();
	OnCreate();
}