#include "GameManager.h"

GameManager::GameManager(): window(nullptr), currentScene(nullptr), 
							fps(60), isRunning(false)
{
}

GameManager::~GameManager()
{
	delete window;
	window = nullptr;
	delete currentScene;
	currentScene = nullptr;
}

void GameManager::Init()
{
	//timer = Timer();
	window = new Window();
	window->OnCreate("GAME", 800, 600);

	currentScene = new Scene1();
	currentScene->OnCreate();
	isRunning = true;
}

void GameManager::Run()
{

	while (isRunning)
	{
		Timer::UpdateTimer();
		currentScene->Update(Timer::GetScaledDeltaTime());
		currentScene->Render();
		HandleEvents();
		SDL_GL_SwapWindow(window->GetWindow());
		SDL_Delay(Timer::SleepTime(fps));
	}

}

void GameManager::HandleEvents()
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		if (event.type == SDL_EventType::SDL_QUIT)
		{
			isRunning = false;
			return;
		}


		currentScene->HandleEvents(event);
	}

}
