#ifndef SCENEAi_H
#define SCENEAi_H

#include "core/scene/Scene.h"

//! Scene Ai class
/*!  This class inherits from Scene and controls all events and object's in the first scene */
class SceneAi : public Scene
{
private:

	Debug debug;
public:
	//! Scene Ai Constructor
	SceneAi() = default;


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
	void HandleEvents(const SDL_Event& event) override;

	//! Virtual overridden Reset Function
	/*! When called resets the scene back to its original state */
	void Reset() override;
};
#endif