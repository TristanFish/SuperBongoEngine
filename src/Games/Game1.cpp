#include "Game1.h"
#include "core/CoreEngine.h"
#include "scenes/Scene1.h"
#include "scenes/Scene2.h"

Game1::Game1() : GameInterface(), currentSceneNum(0), currentScene(nullptr)
{
}

Game1::~Game1()
{
	currentScene->OnDestroy();
	currentScene = nullptr;
}

bool Game1::OnCreate()
{
	if (CoreEngine::GetInstance()->GetCurrentScene() == 0)
	{
		currentScene = new Scene2();
		currentSceneNum = 0;
		return currentScene->OnCreate();
	}
	EngineLogger::Error("Engine's scene is not initialized to 0", "Game1.cpp", __LINE__);
	return false;
}

void Game1::OnDestroy()
{
}

void Game1::Update(const float deltaTime_)
{
	if (currentSceneNum != CoreEngine::GetInstance()->GetCurrentScene())
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
	currentScene->OnDestroy();
	currentScene = nullptr;

	switch (CoreEngine::GetInstance()->GetCurrentScene())
	{
	case 0:
		currentScene = new Scene2();
		break;
	default:
		currentScene = new Scene1();
		break;
	}

	currentSceneNum = CoreEngine::GetInstance()->GetCurrentScene();
	if (!currentScene->OnCreate())
	{
		EngineLogger::Error("Scene failed to be created", "Game1.cpp", __LINE__);
		CoreEngine::GetInstance()->OnDestroy();
	}
}