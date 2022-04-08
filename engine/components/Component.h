#ifndef COMPONENT_H
#define COMPONENT_H

#include "SDL_events.h"
#include <typeinfo>
#include <iostream>
#include <core/resources/SaveManager.h>

//! Component Class
/*!Component is an interface, 
Check Player.h to see how to add components*/

class GameObject;

class Component
{
public:

	//!GameObject Pointer
	/*!Enables the components to know what gameObject they are attached too*/
	GameObject* gameObject;

	bool active = true;
	//!Virtual Init Function
	/*!Initializes all of the needed variables*/
	virtual void Init(GameObject *g) = 0;

	//!Virtual Update Function
	/*!Updates all of the components systems*/
	virtual void Update(const float deltaTime) = 0;

	//!Virtual HandleEvents Function
	/*!Handles any events needed for the component*/
	virtual void HandleEvents(const SDL_Event& event) = 0;

	virtual void OnSaveComponent(const std::string& saveName,std::string parentName);

	virtual void ImGuiRender() {}

	virtual const char* GetType() { return typeid(*this).name(); }


	std::string GetComponentName();

	//!Virtual Destructor 
	/*!Destroys any of the pointers/vectors needed*/
	virtual ~Component() { gameObject = nullptr; }
};

#endif
