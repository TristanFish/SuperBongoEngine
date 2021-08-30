#ifndef COMPONENT_H
#define COMPONENT_H

#include "SDL_events.h"
#include <typeinfo>

//! Component Class
/*!Component is an interface, 
Check Player.h to see how to add components*/

class GameObject;

class Component
{
public:

	//!GameObject Pointer
	/*!Enables the components to know what gameobject they are attached too*/
	GameObject* gameobject;

	bool active = true;
	//!Virtual Init Function
	/*!Initializes all of the needed variables*/
	virtual void Init(GameObject *g) = 0;

	//!Virtual Update Function
	/*!Updates all of the components systems*/
	virtual void Update(const float deltaTime) = 0;

	//!Virtual Render Function
	/*!Renders anything needed for the component*/
	virtual void Render() const {}

	//!Virtual HandleEvents Function
	/*!Handles any events needed for the component*/
	virtual void HandleEvents(const SDL_Event& event) = 0;

	virtual const char* GetType() { return typeid(*this).name(); }

	//!Virtual Destructor 
	/*!Destroys any of the pointers/vectors needed*/
	virtual ~Component() { gameobject = nullptr; }
};



#endif
