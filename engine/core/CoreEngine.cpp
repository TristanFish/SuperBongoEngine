#include "CoreEngine.h"

#include <Windows.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include <ImGuizmo/ImGuizmo.h>

#include <sdl/SDL.h>

#include "Globals.h"
#include "Timer.h"
#include "core/GameInterface.h"
#include "core/Logger.h"
#include "core/scene/Scene.h"
#include "events/InputManager.h"
#include "events/MouseEventDispatcher.h"
#include "graphics/Window.h"

#include "resources/ModelManager.h"
#include "resources/ShaderManager.h"
#include "resources/TextureManager.h"

#include "components/GameObject.h"
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

	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	
	
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
	if (!window->OnCreate("Super Bongo Engine", Globals::Engine::SCREEN_WIDTH, Globals::Engine::SCREEN_HEIGHT))
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
	Globals::Engine::InitGlobals();
	Globals::Engine::SCENE_NAME = GetCurrentScene()->GetSceneName();
	
	dockSpace = new CustomUI::DockSpace();
	dockSpace->ConstructUserInterface();
	CustomUI::PerformanceMonitor::InitMonitor();

	LoadUtility::GetInstance()->LoadDefaultScenes(gameInterface);
	LoadUtility::GetInstance()->LoadSceneSaves();
	LoadSceneData();

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

	SaveSceneData();
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
			case SDL_SCANCODE_Q:
				Globals::Editor::GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case SDL_SCANCODE_R:
				Globals::Editor::GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case SDL_SCANCODE_E:
				Globals::Editor::GizmoType = ImGuizmo::OPERATION::SCALE;
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

void CoreEngine::SaveSceneData() const
{
	std::string Scene_Name = gameInterface->currentScene->GetSceneName();

	if (!SaveManager::TransferToSaveQueue(Scene_Name))
	{
		SaveUtility::GetInstance()->CreateSave(Scene_Name, FileType::SCENE);
	}

	SaveManager::GetSaveFile(Scene_Name).ClearElements();
	ElementInfo info = ElementInfo("Root");

	SaveUtility::GetInstance()->AddElement(Scene_Name, "SceneSettings", info);
	info = ElementInfo("SceneSettings");
	info.Attributes.emplace("S_:", std::string(typeid(*this).name()));
	SaveUtility::GetInstance()->AddElement(Scene_Name, "BaseClass:", info);

	info = ElementInfo("Root");
	SaveUtility::GetInstance()->AddElement(Scene_Name, "Objects", info);

	info = ElementInfo("Objects");

	for (const auto& obj : Globals::Engine::s_SceneGraph->GetGameObjects())
	{
		SaveUtility::GetInstance()->AddElement(Scene_Name, obj->GetName(), info);
		SaveUtility::GetInstance()->SaveObject(obj->GetName(), obj);
	}

	SaveUtility::GetInstance()->CompileSaves();
}

void CoreEngine::LoadSceneData()
{
	for (auto elm : SaveManager::GetSaveFile(gameInterface->currentScene->GetSceneName()).GetElements())
	{
		if (!Globals::Engine::s_SceneGraph->FindGameObject(elm.first))
		{
			LoadUtility::GetInstance()->LoadObject(SaveManager::GetSaveFile(elm.first));
		}
	}
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

