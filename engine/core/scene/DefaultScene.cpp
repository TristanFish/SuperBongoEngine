
#include "DefaultScene.h"

#include "core/MouseRay.h"
#include "core/Logger.h"
#include "core/resources/SaveManager.h"
#include "core/Globals.h"



DefaultScene::DefaultScene(std::string Scene_Name_) 
{
	Scene_Name = Scene_Name_;
}

DefaultScene::DefaultScene()
{
	Scene_Name = "Default";
}

bool DefaultScene::OnCreate()
{
	EngineLogger::Info(Scene_Name + " Created", "DefaultScene.cpp", __LINE__);


	Globals::InitGlobals();

	LoadMapData();


	objectList->Init();



	return true;
}

void DefaultScene::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
	Camera::getInstance()->Update(deltaTime);
}

void DefaultScene::Render()
{
	Scene::Render();
}

void DefaultScene::HandleEvents(const SDL_Event& event)
{
	Scene::HandleEvents(event);
}

void DefaultScene::Reset()
{
	OnDestroy();
	OnCreate();
}