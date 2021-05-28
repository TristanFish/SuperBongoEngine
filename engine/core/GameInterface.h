#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <sdl/SDL.h>

class GameInterface
{
public:
	
	class Scene* currentScene;

	
	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime_) = 0;
	virtual void Render() = 0;
	virtual void HandleEvents(const SDL_Event& event) = 0;
};

#endif