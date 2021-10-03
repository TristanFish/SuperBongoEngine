#include "Scene2.h"
#include "core/CoreEngine.h"
#include "core/Globals.h"



Scene2::Scene2()
{
	Scene_Name = "Scene_2";
}

bool Scene2::OnCreate()
{
	EngineLogger::Info("Scene 2 Created", "Scene2.cpp", __LINE__);

	Globals::InitGlobals();
	objectList->Init();

	return true;
}

void Scene2::Reset()
{
	OnDestroy();
	OnCreate();
}

void Scene2::Update(float deltaTime_)
{
	Scene::Update(deltaTime_);
	Camera::getInstance()->Update(deltaTime_);

}

void Scene2::HandleEvents(const SDL_Event& event_)
{
	Scene::HandleEvents(event_);

	
}

void Scene2::Render() 
{
	Scene::Render();
}
