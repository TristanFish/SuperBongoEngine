#ifndef BIRD_H
#define BIRD_H

#include "components/GameObject.h"
#include "core/Debug.h"
#include "fmod/fmod.hpp"

class Bird : public GameObject
{
public:

	Bird(const std::string& name, MATH::Vec3 position);
	~Bird() = default;

	std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<Bird>(this->name, this->transform.GetPosition()); };


	//! Base Grass Constructor
private:

	FMOD::Sound* birdSound;
	//! Secondary Grass Constructor
	/*! This is the main constructor that we will use and takes in a char* for our gameObject name
	and a Vec3 Position for where our object will be in world space and a unsigned int amount that determents
	the number of instances we want to render*/

	//! Base Grass Destructor

	Debug frame;

};

#endif