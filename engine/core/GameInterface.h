#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <sdl/SDL.h>
#include <vector>

class Scene;

class Game
{
public:
	
	 Scene* currentScene;

	std::vector<Scene*> Scenes;

	int currentSceneNum;

	
	virtual ~Game();
	virtual bool BuildScene();
	virtual bool OnCreate() = 0;
	virtual void OnDestroy();
	virtual void Update(const float deltaTime_);
	virtual void Render();
	virtual void HandleEvents(const SDL_Event& event);
	virtual void PlayCurrentScene();
};

#endif