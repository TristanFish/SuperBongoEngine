#include "SceneAi.h"
#include "core/Logger.h"
#include "core/resources/SaveManager.h"
#include "core/Globals.h"


#include "gameObjects/AiGameObject.h"

#include "Primitives/Sphere.h"



bool SceneAi::OnCreate()
{
	EngineLogger::Info("Scene Ai Created", "SceneAi.cpp", __LINE__);



	Globals::InitGlobals();
	CustomUI::PerformanceMonitor::InitMonitor();



	objectList->Init();

	AiGameObject* mainAi = new AiGameObject("AiObj",Vec3(1.0f));
	objectList->AddGameObject(mainAi);

	Sphere* targetBox = new Sphere("targetSphereObj",Vec3(5.0f, 2.5f, 7.0f));
	objectList->AddGameObject(targetBox);

	mainAi->aiTarget = targetBox;
	
	return true;
}

void SceneAi::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
	Camera::getInstance()->Update(deltaTime);
}

void SceneAi::Render()
{
	Scene::Render();
}

void SceneAi::HandleEvents(const SDL_Event& event)
{
	Scene::HandleEvents(event);
}

void SceneAi::Reset()
{
	OnDestroy();
	OnCreate();
}
