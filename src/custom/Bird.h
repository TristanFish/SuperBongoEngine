#ifndef BIRD_H
#define BIRD_H

#include "components/Components.h"

class Bird : public GameObject, public AudioSourceComponent, public MeshRenderer
{
public:

	Bird(const char* name, MATH::Vec3 position);
	Bird();
	//! Base Grass Constructor
private:

	FMOD::Sound* birdSound;
	//! Secondary Grass Constructor
	/*! This is the main constructor that we will use and takes in a char* for our gameobject name
	and a Vec3 Position for where our object will be in world space and a unsigned int amount that determents
	the number of instances we want to render*/

	//! Base Grass Destructor
	~Bird();

	//! Update Function (Inherited from Gameobject)
	void Update(const float deltaTime) override;

	//! Render Function (Inherited from Gameobject)
	void Render() const override;

	//! Handle Events Function (Inherited from Gameobject)
	void HandleEvents(const SDL_Event& event) override;

};

#endif