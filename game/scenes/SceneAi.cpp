#include "SceneAi.h"


#include "Scene1.h"
#include "core/Logger.h"
#include "core/Globals.h"
#include "gameObjects/AiGameObject.h"
#include "Primitives/Sphere.h"
#include "Primitives/Box.h"


bool SceneAi::OnCreate()
{
	Scene_Name = "Scene_AI";
	EngineLogger::Info("Scene Ai Created", "SceneAi.cpp", __LINE__);

	mainAi = std::make_shared<AiGameObject>("AiObj", Vec3(1.0f));
	targetSphere = std::make_shared<Sphere>("targetSphere", Vec3(10.0f, 2.5f, 7.0f));
	
	objectList->AddGameObject(mainAi);

	objectList->AddGameObject(targetSphere);
	//
	mainAi->aiTarget = targetSphere;

	

	

	objectList->Init();

	
	Scene1().OnCreate();
	
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
