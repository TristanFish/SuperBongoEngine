#include "GameInterface.h"

#include "core/CoreEngine.h"
#include "core/Globals.h"
#include "core/scene/DefaultScene.h"

#include "Utility/LoadUtility.h"

Game::~Game()
{
	for (auto* scene : Scenes)
	{
		delete scene;
		scene = nullptr;
	}
}

bool Game::BuildScene()
{
	currentScene = Scenes[CoreEngine::GetInstance()->GetCurrentSceneNum()];
	currentSceneNum = CoreEngine::GetInstance()->GetCurrentSceneNum();
	
	currentScene->Reset();
	Renderer::GetInstance()->ClearComponents();
	
	LoadUtility::GetInstance()->LoadSceneSaves();
	Globals::Engine::InitGlobals();
	if (!currentScene->OnCreate())
	{
		EngineLogger::Error("Scene failed to be created", "Game1.cpp", __LINE__);
		return false;
	}
	CoreEngine::GetInstance()->LoadSceneData();
	if(!currentScene->PostCreate())
	{
		EngineLogger::Error("Scene failed on PostCreate", "Game1.cpp", __LINE__);
		return false;
	}
	return true;
}

void Game::OnDestroy()
{
}

void Game::Update(const float deltaTime_)
{
	if (currentSceneNum != CoreEngine::GetInstance()->GetCurrentSceneNum())
	{
		if(!BuildScene())
		{
			EngineLogger::FatalError("Scene Building failed closing engine", "Game1.cpp", __LINE__);
			CoreEngine::GetInstance()->SetIsGameRunning(false);
			return;
		}
	}
	currentScene->Update(deltaTime_);
}

void Game::Render()
{
	currentScene->Render();
}

void Game::HandleEvents(const SDL_Event& event)
{
	currentScene->HandleEvents(event);
}
