#ifndef SCENE1_H
#define SCENE1_H

#include "core/scene/Scene.h"

//! Scene 1 class
/*!  This class inherits from Scene and controls all events and object's in the first scene */
class Scene1 : public Scene
{
public:
	//! Scene 1 Constructor
	Scene1() = default;


	//! Virtual overridden OnCreate Function
	/*! Gets called at the beginning of the loop */
	bool OnCreate() override;

	//! Virtual overridden OnDestroy Function
	/*! Gets called at the end of the loop */
	void OnDestroy() override {}

	//! Virtual overridden Update Function
	/*! Updates all of the gameobjects throughout the loop */
	void Update(const float deltaTime) override;

	//! Virtual overridden Render Function
	/*! Renders all of the gameobjects throughout the loop */
	void Render() override;

	//! Virtual overridden HandleEvents Function
	/*! Checks for keyboard/mouse events throughout the loop */
	void HandleEvents(const SDL_Event &event) override;

	//! Virtual overridden Reset Function
	/*! When called resets the scene back to its original state */
	void Reset() override;
};
#endif