#ifndef SCENE_H
#define SCENE_H

#include "sdl/SDL.h"
#include "components/ECS.h"
#include "glew/glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

class Scene
{
public:

	Manager* objectList;

	Scene() {};
	~Scene() {};

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event& event) = 0;
	virtual void Reset() = 0;


};

#endif