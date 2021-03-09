#pragma once
#include "components/ECS.h"
#include "components/Components.h"
#include "custom/Instancer.h"

//! Test Model class
/*! This class is a gameobject that has a MeshRenderer and a RigidBody3D.
This object can also be spawned in engine  */
class TestModel : public GameObject
{
public:

	//! Secondary Test Model Constructor
	/*! This is the main constructor that we will use and takes in a char* for our gameobject name 
	and a Vec3 Position for where our object will be in world space*/
	TestModel(const char* name, MATH::Vec3 position);

	//! Test Model Destructor
	~TestModel() override = default;

	//! Attach Uniforms Function
	/*! This virtual function can be overridden to allow us to send custom variables to the shader*/
	void operator()() override;

	const char* GetClassIDName() const override;

private:

	MeshRenderer* mr;

};

