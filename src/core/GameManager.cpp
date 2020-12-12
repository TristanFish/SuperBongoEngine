#include "GameManager.h"
#include "custom/Camera.h"
#include <iostream>
#include "Globals.h"
#include "core/ModelManager.h"
#include "core/TextureManager.h"
#include "core/InputManager.h"
#include "graphics/CustomUI.h"

GameManager::GameManager(): window(nullptr), currentScene(nullptr), 
							fps(60), isRunning(false)
{
}

GameManager::~GameManager()
{
	delete window;
	window = nullptr;
	currentScene->OnDestroy();
	delete currentScene;
	currentScene = nullptr;
	Camera::removeInstance();
}

void GameManager::Init()
{
	//timer = Timer();
	window = new Window();
	window->OnCreate("Super Bongo Engine", Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);


	TextureManager::LoadAllTextures();
	ModelManager::LoadAllModels();

	currentScene = new Scene1();
	currentScene->OnCreate();
	isRunning = true;
}

void GameManager::Run()
{
	
	Timer::UpdateTimer();

	while (isRunning)
	{
		Uint32 timeBeforeUpdate = SDL_GetTicks();
		Timer::UpdateTimer();
		currentScene->Update(Timer::GetScaledDeltaTime());
		HandleEvents();
		Uint32 timeAfterUpdate = SDL_GetTicks();

		PerformanceMonitor::UpdateLoopTime = (timeAfterUpdate - timeBeforeUpdate);


		Uint32 timebeforeRender = SDL_GetTicks();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window->GetWindow());


		currentScene->Render();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window->GetWindow());

		Uint32 timeafterRender = SDL_GetTicks();

		PerformanceMonitor::RenderLoopTime = (timeafterRender - timebeforeRender);

		if (PerformanceMonitor::LimitFPS)
		{
			SDL_Delay(Timer::SleepTime(PerformanceMonitor::FPSLimit));
		}

	}

	TextureManager::DeleteAllTextures();
	ModelManager::DestroyAllModels();
	InputManager::RemoveInstance();
	SDL_Quit();
}

void GameManager::HandleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		InputManager::GetInstance()->PollEvents(event);

		if (event.type == SDL_EventType::SDL_QUIT)
		{
			isRunning = false;
			return;
		}
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
				isRunning = false;
				std::cout << "Closing Game" << std::endl;
				return;
			case SDL_SCANCODE_P:
				ReloadCurrentScene();
				break;
			}
		}

		currentScene->HandleEvents(event);
	}

}

void GameManager::ReloadCurrentScene()
{
	currentScene->Reset();
}

