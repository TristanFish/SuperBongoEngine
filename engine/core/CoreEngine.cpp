#include "CoreEngine.h"
#include "Globals.h"
#include "resources/ShaderManager.h"
#include "resources/ModelManager.h"
#include "resources/TextureManager.h"
#include "events/InputManager.h"
#include "graphics/Window.h"
#include "Timer.h"
#include "core/scene/Scene.h"
#include "core/Logger.h"
#include "core/GameInterface.h"
#include "events/MouseEventDispatcher.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include <sdl/SDL.h>
#include <Windows.h>

#include "Concurency/ThreadHandler.h"

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

	std::shared_ptr<Task> HandleEventsTask = std::make_shared<Task>(ETaskPriority::High, ETaskType::TT_RENDERING);
	HandleEventsTask->SetTask(&CoreEngine::HandleEvents, this);
	ThreadHandler::GetInstance()->AddTask(HandleEventsTask, ETaskFlags::TF_WAITFORFINISH);
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

	std::shared_ptr<Task> CreateWindowTask = std::make_shared<Task>(ETaskPriority::Medium, ETaskType::TT_RENDERING);
	CreateWindowTask->SetTask(&Window::OnCreate, window, "Super Bongo Engine", Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);

	/*if (!window->OnCreate("Super Bongo Engine", Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT))
	{
		EngineLogger::FatalError("Window failed to initialize", "CoreEngine.cpp", __LINE__);
		OnDestroy();
		return false;
	}*/
	

	SDL_WarpMouseInWindow(window->GetWindow(), window->GetWidth() / 2, window->GetHeight() / 2);

	auto before_time = std::chrono::high_resolution_clock::now();


	NetworkManager::GetInstance()->Init();
	TextureManager::GetInstance()->LoadAllTextures();
	ModelManager::GetInstance()->LoadAllModels();

	std::shared_ptr<Task> LoadSavesTask = std::make_shared<Task>(ETaskPriority::Medium,ETaskType::TT_RENDERING);
	LoadSavesTask->SetTask(&LoadUtility::LoadExistingSaves, LoadUtility::GetInstance());

	

	if (gameInterface)
	{

		std::shared_ptr<Task> CreateInterfaceTask = std::make_shared<Task>(ETaskPriority::Medium, ETaskType::TT_RENDERING);
		CreateInterfaceTask->SetTask(&GameInterface::OnCreate, gameInterface);

		//Find a way to properly return values (Futures?)
		ThreadHandler::GetInstance()->AddStrand(std::make_shared<Strand>(std::vector{CreateWindowTask,LoadSavesTask, CreateInterfaceTask }));

		/*if (!gameInterface->OnCreate())
		{
			EngineLogger::FatalError("Game failed to initialize", "CoreEngine.cpp", __LINE__);
			OnDestroy();
			return false;
		}*/

	}

	LoadUtility::GetInstance()->LoadDefaultScenes(gameInterface);

	std::shared_ptr<Task> LoadSceneSavesTask = std::make_shared<Task>(ETaskPriority::Medium, ETaskType::TT_RENDERING);
	LoadSceneSavesTask->SetTask(&LoadUtility::LoadSceneSaves, LoadUtility::GetInstance());
	ThreadHandler::GetInstance()->AddTask(LoadSceneSavesTask,ETaskFlags::TF_WAITFORFINISH);

	Globals::SCENE_NAME = GetCurrentScene()->GetSceneName();

	std::shared_ptr<Task> LoadMapDataTask = std::make_shared<Task>(ETaskPriority::Medium, ETaskType::TT_RENDERING);
	LoadMapDataTask->SetTask(&Scene::LoadMapData, GetCurrentScene());
	ThreadHandler::GetInstance()->AddTask(LoadMapDataTask, ETaskFlags::TF_WAITFORFINISH);
	
	isRunning = true;
	return true;
}

void CoreEngine::Run()
{
	
	Timer::UpdateTimer();

	std::shared_ptr<Task> RenderTask = std::make_shared<Task>(ETaskPriority::High, ETaskType::TT_RENDERING);
	while (isRunning)
	{

		auto timeBeforeUpdate = std::chrono::high_resolution_clock::now();
		Timer::UpdateTimer();
		Update(Timer::GetDeltaTime());
		auto timeAfterUpdate = std::chrono::high_resolution_clock::now();


		auto executeTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeAfterUpdate - timeBeforeUpdate);
		CustomUI::PerformanceMonitor::UpdateLoopTime = static_cast<float>(executeTime.count());


		auto timebeforeRender = std::chrono::high_resolution_clock::now();
		RenderTask->SetTask(&CoreEngine::Render, this);
		ThreadHandler::GetInstance()->AddTask(RenderTask,ETaskFlags::TF_WAITFORFINISH);
		auto timeafterRender = std::chrono::high_resolution_clock::now();


		executeTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeafterRender - timebeforeRender);
		CustomUI::PerformanceMonitor::RenderLoopTime = static_cast<float>(executeTime.count());
		
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
	TextureManager::GetInstance()->DeleteAllTextures();
	ModelManager::GetInstance()->DestroyAllModels();
	InputManager::RemoveInstance();
	ShaderManager::DestroyAllShaders();

	

	SDL_Quit();
}

void CoreEngine::PrintTest(int i)
{
	std::cout << "Something Worked! "<< i << std::endl;
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

Window* CoreEngine::GetWindow() const
{
	return window;
}

void CoreEngine::ReloadCurrentScene()
{
	
}

