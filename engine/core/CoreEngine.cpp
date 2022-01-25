#include "CoreEngine.h"

#include <Windows.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>
#include <sdl/SDL.h>

#include "Globals.h"
#include "Timer.h"
#include "core/GameInterface.h"
#include "core/Logger.h"
#include "core/scene/Scene.h"

#include "components/AI/Algorithms/Pathfinding/Types/Graph.h"

#include "events/InputManager.h"
#include "events/MouseEventDispatcher.h"
#include "graphics/Window.h"

#include "resources/ModelManager.h"
#include "resources/ShaderManager.h"
#include "resources/TextureManager.h"
#include "resources/AIDirector.h"
#include "Utility/LoadUtility.h"
std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

CoreEngine::CoreEngine(): window(nullptr), isRunning(false), fps(60), currentSceneNum(0), gameInterface(nullptr)
{

}

CoreEngine::~CoreEngine()
{
}

void CoreEngine::Update(const float deltaTime_)
{
	dockSpace->Update(deltaTime_);
	if (gameInterface)
	{
		gameInterface->Update(deltaTime_);
	}
	HandleEvents();
}

void CoreEngine::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();

	dockSpace->Render();
	if (gameInterface)
	{
		gameInterface->Render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
	SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(backup_current_window, backup_current_context);

	SDL_GL_SwapWindow(window->GetWindow());
}

CoreEngine* CoreEngine::GetInstance()
{
	if (engineInstance == nullptr)
	{
		engineInstance.reset(new CoreEngine);
	}
	return engineInstance.get();
}

bool CoreEngine::Init()
{
	EngineLogger::OnCreate();
	EngineLogger::Info("Logger Created", "CoreEngine.cpp", __LINE__);
	
	window = new Window();
	if (!window->OnCreate("Super Bongo Engine", Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT))
	{
		EngineLogger::FatalError("Window failed to initialize", "CoreEngine.cpp", __LINE__);
		OnDestroy();
		return false;
	}

	SDL_WarpMouseInWindow(window->GetWindow(), window->GetWidth() / 2, window->GetHeight() / 2);


	NetworkManager::GetInstance()->Init();
	TextureManager::LoadAllTextures();
	ModelManager::LoadAllModels();
	LoadUtility::GetInstance()->LoadExistingSaves();
	Renderer::GetInstance()->Init();
	
	if (gameInterface)
	{
		if (!gameInterface->OnCreate())
		{
			EngineLogger::FatalError("Game failed to initialize", "CoreEngine.cpp", __LINE__);
			OnDestroy();
			return false;
		}

	}
	Globals::InitGlobals();
	dockSpace = new CustomUI::DockSpace();
	dockSpace->ConstructUserInterface();
	CustomUI::PerformanceMonitor::InitMonitor();

	LoadUtility::GetInstance()->LoadDefaultScenes(gameInterface);
	LoadUtility::GetInstance()->LoadSceneSaves();
	Globals::SCENE_NAME = GetCurrentScene()->GetSceneName();
	//GetCurrentScene()->LoadMapData();


	isRunning = true;
	return true;
}

void CoreEngine::Run()
{
	Timer::UpdateTimer();

	while (isRunning)
	{
		const auto timeBeforeUpdate = std::chrono::high_resolution_clock::now();
		Timer::UpdateTimer();
		Update(Timer::GetDeltaTime());

	
		const auto timeAfterUpdate = std::chrono::high_resolution_clock::now();
		const auto executeTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterUpdate - timeBeforeUpdate);

		CustomUI::PerformanceMonitor::UpdateLoopTime = static_cast<float>(executeTime.count());

		const auto timeBeforeRender = std::chrono::high_resolution_clock::now();
		Render();
		const auto timeAfterRender = std::chrono::high_resolution_clock::now();
		const auto renderTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterRender - timeBeforeRender);

		CustomUI::PerformanceMonitor::RenderLoopTime = static_cast<float>(renderTime.count());

		if(limitfps)
		{
			SDL_Delay(Timer::SleepTime(fps));
		}
	}

	gameInterface->currentScene->SaveMapData();
	OnDestroy();
}

bool CoreEngine::GetIsRunning() const
{
	return isRunning;
}

void CoreEngine::HandleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		InputManager::GetInstance()->PollEvents(event);
		MouseEventDispatcher::Update(event);
		
		if (event.type == SDL_EventType::SDL_QUIT)
		{
			isRunning = false;
			return;
		}

		if (event.type == SDL_KEYDOWN) 
		{
			switch (event.key.keysym.scancode)
			{
			case SDL_SCANCODE_ESCAPE:
				isRunning = false;
				EngineLogger::Info("Closing Game", "CoreEngine.cpp", __LINE__);
				return;
			case SDL_SCANCODE_P:
				ReloadCurrentScene();
				break;
			default:
				break;
			}
		}
		
		if (gameInterface)
		{
			gameInterface->HandleEvents(event);
		}
	}
}

void CoreEngine::OnDestroy()
{
	if (gameInterface)
	{
		delete gameInterface;
		gameInterface = nullptr;
	}
	if (window)
	{
		delete window;
		window = nullptr;
	}
	if(dockSpace)
	{
		delete dockSpace;
		dockSpace = nullptr;
	}
	Renderer::GetInstance()->DestroyRenderer();
	Camera::removeInstance();
	TextureManager::DeleteAllTextures();
	ModelManager::DestroyAllModels();
	InputManager::RemoveInstance();
	ShaderManager::DestroyAllShaders();
	SaveManager::DeleteSaveableObjects();

	SDL_Quit();
}

void CoreEngine::SetGameInterface(GameInterface* gameInterface_)
{
	gameInterface = gameInterface_;
}

int CoreEngine::GetCurrentSceneNum() const
{
	return currentSceneNum;
}

Scene* CoreEngine::GetCurrentScene() const
{
	return gameInterface->currentScene;
}

void CoreEngine::ReloadCurrentScene()
{
	
}

