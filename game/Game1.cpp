#include "Game1.h"
#include "core/CoreEngine.h"
#include "scenes/Scene1.h"
#include "scenes/Scene2.h"
#include "core/scene/DefaultScene.h"

#include "scenes/SceneAi.h"

Game1::Game1() : currentSceneNum(0)
{
	currentScene = nullptr;
	Scenes = std::vector<Scene*>();
}

Game1::~Game1()
{
	for (auto scene : Scenes)
	{
		delete scene;
		scene = nullptr;
	}
}

bool Game1::OnCreate()
{
	if (CoreEngine::GetInstance()->GetCurrentScene() == 0)
	{
		currentScene = new Scene1(); //this should be Scene1. Change this only if you want to temp load a scene from code
		currentSceneNum = 0;
		bool create = currentScene->OnCreate();
		bool postCreate = currentScene->PostCreate();

		Scenes.push_back(currentScene);
		Scenes.push_back(new Scene2);


		

		return (create && postCreate);
	}
	EngineLogger::Error("Engine's scene is not initialized to 0", "Game1.cpp", __LINE__);
	return false;
}

void Game1::OnDestroy()
{

}

void Game1::Update(const float deltaTime_)
{
	if (currentSceneNum != CoreEngine::GetInstance()->GetCurrentSceneNum())
	{
		BuildScene();
	}
	currentScene->Update(deltaTime_);
}

void Game1::Render()
{
	currentScene->Render();
}

void Game1::HandleEvents(const SDL_Event& event)
{
	currentScene->HandleEvents(event);
}

void Game1::BuildScene() 
{
	//delete currentScene;
	currentScene = nullptr;


	currentScene = Scenes[CoreEngine::GetInstance()->GetCurrentSceneNum()];
	

	currentSceneNum = CoreEngine::GetInstance()->GetCurrentSceneNum();
	
	Renderer::ResetInstance();
	
	LoadUtility::GetInstance()->LoadSceneSaves();
	if (!currentScene->OnCreate())
	{
		EngineLogger::Error("Scene failed to be created", "Game1.cpp", __LINE__);
		CoreEngine::GetInstance()->OnDestroy();
	}
	currentScene->LoadMapData();
	currentScene->objectList->AddRenderingComponents();
	if(!currentScene->PostCreate())
	{
		EngineLogger::Error("Scene failed on PostCreate", "Game1.cpp", __LINE__);
		CoreEngine::GetInstance()->OnDestroy();
	}

}