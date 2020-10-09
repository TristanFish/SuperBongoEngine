#ifndef SCENE_H
#define SCENE_H

#include "sdl/SDL.h"
#include "components/ECS.h"
#include "glew/glew.h"
#include "core/tinyxml2.h"
#include <stdio.h>
using namespace tinyxml2;

class Scene
{
public:

	Manager* objectList;

	// Used for saving and loading xml document information 
	XMLElement* pElement;
	Scene() {};
	~Scene() {};

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event& event) = 0;
	virtual void Reset() = 0;
	virtual void SaveMapData();
	virtual void LoadMapData();


	const char* CheckAtributeValue(int i);
	
};

#endif