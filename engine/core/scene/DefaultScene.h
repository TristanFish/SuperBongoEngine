#ifndef DEFAULTSCENE_H
#define DEFAULTSCENE_H

#include "Scene.h"

//! Scene 1 class
/*!  This class inherits from Scene and controls all events and object's in the first scene */
class DefaultScene : public Scene
{
public:
	//! Scene 1 Constructor
	DefaultScene();
	DefaultScene(const std::string& Scene_Name);


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

};
#endif

