#ifndef SCENE_H
#define SCENE_H

#include "sdl/SDL.h"
#include "components/ECS.h"
#include "glew/glew.h"
#include "core/tinyxml2.h"
#include "imgui/imgui.h"
#include "graphics/CustomUI.h"
#include "VMath.h"
#include <stdio.h>
using namespace tinyxml2;

class MouseRay;

class Scene
{
protected:
	//! MouseRay Pointer
	/*! Converts mouse position on screen to world space to allow us to do mouse picking  */
	MouseRay* mouseRay;

	//! GameObject Pointer
	/*! Stores our object that was most recently selected using our mouseRay */
	std::vector<CustomUI::PropertiesPanel*> propertiesPannels;

	//! Const Char* Pointer
	/*! Stores our objects that can be spawned */
	const char* objClasses[2] = { "Plane", "Test Object" };

	//! Create object with object ID function
	/*! Used when we want to runtime spawn objects depending on the given ID */
	void CreateObjWithID(Vec3 pos_, Vec3 rot_, Vec3 scale_, const char* name_, const int& ID) const;

	//! Check Intersection function
	/*! Checks if the MouseRay has intersected with a object */
	bool CheckIntersection(MouseRay* ray, const Vec3& origin, GameObject* obj);

public:
	Manager* objectList;

	// Used for saving and loading xml document information 
	XMLElement* pElement;
	Scene() {};
	~Scene() {};

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event& event);
	virtual void Reset() = 0;
	virtual void SaveMapData() const;
	virtual void LoadMapData();


	const char* CheckAtributeValue(int i) const;
	
};

#endif