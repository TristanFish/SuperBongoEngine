#include "SceneAi.h"

#include "core/Logger.h"
#include "core/Globals.h"
#include "gameObjects/AiGameObject.h"
#include "Primitives/Sphere.h"

bool SceneAi::OnCreate()
{
	Scene_Name = "Scene_AI";
	EngineLogger::Info("Scene Ai Created", "SceneAi.cpp", __LINE__);

	Globals::InitGlobals();
	CustomUI::PerformanceMonitor::InitMonitor();

	objectList->Init();

	AiGameObject* mainAi = new AiGameObject("AiObj",Vec3(1.0f));
	objectList->AddGameObject(mainAi);

	Sphere* targetSphere = new Sphere("targetSphere",Vec3(15.0f, 2.5f, 7.0f));
	objectList->AddGameObject(targetSphere);
	
	mainAi->aiTarget = targetSphere;

	
	return true;
}

void SceneAi::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
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
