
#include "Scene1.h"
#include "gameObjects/Player.h"
#include "core/MouseRay.h"
#include "core/Logger.h"
#include "gameObjects/LightObject.h"
#include "gameObjects/Bird.h"
#include "Primitives/PlaneObject.h"
#include "gameObjects/Grass.h"
#include "core/resources/SaveManager.h"

bool Scene1::OnCreate()
{
	EngineLogger::Info("Scene 1 Created", "Scene1.cpp", __LINE__);


	LoadUtility::GetInstance()->LoadExistingSaves();



	LoadMapData();

	PerformanceMonitor::InitMonitor();

	objectList->Init();
	return true;

}

void Scene1::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
	Camera::getInstance()->Update(deltaTime);
}

void Scene1::Render() const
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






