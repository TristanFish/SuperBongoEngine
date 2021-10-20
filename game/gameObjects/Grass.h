#pragma once

#include "components/ECS.h"
#include "components/Components.h"
#include "components/Instancer.h"

//! Grass class
/*! This class is a gameObject that has a MeshRenderer and a Instancer 
 An Instancer allows this object to render multiple instances of its desired object*/
class Grass : public GameObject, public Instancer
{
public:

	unsigned int amount;
	
	//! Secondary Grass Constructor
	/*! This is the main constructor that we will use and takes in a char* for our gameObject name
	and a Vec3 Position for where our object will be in world space and a unsigned int amount that determents
	the number of instances we want to render*/
	Grass(std::string name, MATH::Vec3 position, unsigned int amount_);

	//! Base Grass Destructor
	~Grass() = default;

	Grass* GetClone() const override { return new Grass(this->name, this->transform.pos, amount); }

	
};

