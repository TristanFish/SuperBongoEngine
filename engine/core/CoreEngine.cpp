#include "CoreEngine.h"
#include "Globals.h"
#include "resources/ShaderManager.h"
#include "resources/ModelManager.h"
#include "resources/TextureManager.h"
#include "events/InputManager.h"
#include "graphics/Window.h"
#include "Timer.h"
#include "core/scene/Scene.h"
#include "events/MouseEventDispatcher.h"
#include "core/Logger.h"
#include "core/GameInterface.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include <sdl/SDL.h>
#include <Windows.h>
std::unique_ptr<CoreEngine> CoreEngine::engineInstance = nullptr;

CoreEngine::CoreEngine(): window(nullptr), fps(60), isRunning(false), currentSceneNum(0), gameInterface(nullptr)
{

}

CoreEngine::~CoreEngine()
{
	OnDestroy();
}

void CoreEngine::Update(const float deltaTime_)
{
	//currentScene->Update(Timer::GetScaledDeltaTime());
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

	if (gameInterface)
	{
		if (!gameInterface->OnCreate())
		{
			EngineLogger::FatalError("Game failed to initialize", "CoreEngine.cpp", __LINE__);
			OnDestroy();
			return false;
		}

	}

	LoadUtility::GetInstance()->LoadDefaultScenes(gameInterface);
	LoadUtility::GetInstance()->LoadSceneSaves();
	Globals::SCENE_NAME = GetCurrentScene()->GetSceneName();
	//temp GetCurrentScene()->LoadMapData();

	



	isRunning = true;
	return true;
}

void CoreEngine::Run()
{
	
	Timer::UpdateTimer();

	while (isRunning)
	{
		const Uint32 timeBeforeUpdate = SDL_GetTicks();
		Timer::UpdateTimer();
		Update(Timer::GetDeltaTime());
		const Uint32 timeAfterUpdate = SDL_GetTicks();

		CustomUI::PerformanceMonitor::UpdateLoopTime = static_cast<float>(timeAfterUpdate - timeBeforeUpdate);


		const Uint32 timebeforeRender = SDL_GetTicks();
		Render();
		const Uint32 timeafterRender = SDL_GetTicks();

		CustomUI::PerformanceMonitor::RenderLoopTime = static_cast<float>(timeafterRender - timebeforeRender);
		
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
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.scancode) {
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

	Camera::removeInstance();
	TextureManager::DeleteAllTextures();
	ModelManager::DestroyAllModels();
	InputManager::RemoveInstance();
	ShaderManager::DestroyAllShaders();

	

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

