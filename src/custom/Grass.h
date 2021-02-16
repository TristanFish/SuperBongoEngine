#pragma once

#include "components/ECS.h"
#include "components/Components.h"
#include "custom/Rendering/Instancer.h"

//! Grass class
/*! This class is a gameobject that has a MeshRenderer and a Instancer 
 An Instancer allows this object to render multiple instances of its desired object*/
class Grass : public GameObject, public MeshRenderer, Instancer
{
public:

	//! Base Grass Constructor
	Grass();

	//! Secondary Grass Constructor
	/*! This is the main constructor that we will use and takes in a char* for our gameobject name
	and a Vec3 Position for where our object will be in world space and a unsigned int amount that determents
	the number of instances we want to render*/
	Grass(const char* name, MATH::Vec3 position, unsigned int amount_);

	//! Base Grass Destructor
	~Grass();

	//! Update Function (Inherited from Gameobject)
	void Update(const float deltaTime) override;

	//! Render Function (Inherited from Gameobject)
	void Render() const override;

	//! Handle Events Function (Inherited from Gameobject)
	void HandleEvents(const SDL_Event& event) override;

private:

};

