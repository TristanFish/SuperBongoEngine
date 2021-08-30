#ifndef BIRD_H
#define BIRD_H

#include "components/GameObject.h"
#include "core/Debug.h"
#include "fmod/fmod.hpp"

class Bird : public GameObject
{
public:

	Bird(const char* name, MATH::Vec3 position);
	
	//! Base Grass Constructor
private:

	FMOD::Sound* birdSound;
	//! Secondary Grass Constructor
	/*! This is the main constructor that we will use and takes in a char* for our gameobject name
	and a Vec3 Position for where our object will be in world space and a unsigned int amount that determents
	the number of instances we want to render*/

	//! Base Grass Destructor
	~Bird() = default;

	Debug frame;

};

#endif