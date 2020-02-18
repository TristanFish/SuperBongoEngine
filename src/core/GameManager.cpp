#include "GameManager.h"
#include <iostream>

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
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
				isRunning = false;
				std::cout << "Closing Game" << std::endl;
				return;
			}
		}

		currentScene->HandleEvents(event);
	}

}
