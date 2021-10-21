#include "Scene1.h"
#include "core/MouseRay.h"
#include "core/Logger.h"
#include "core/Globals.h"
#include "gameObjects/LightObject.h"


bool Scene1::OnCreate()
{
	EngineLogger::Info("Scene 1 Created", "Scene1.cpp", __LINE__);



	Globals::InitGlobals();
	CustomUI::PerformanceMonitor::InitMonitor();
	


	objectList->Init();



	return true;
}

void Scene1::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
	Camera::getInstance()->Update(deltaTime);
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






