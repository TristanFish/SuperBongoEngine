#pragma once
#include "components/ECS.h"
#include "components/Components.h"
#include "components/Instancer.h"

//! Test Model class
/*! This class is a gameObject that has a MeshRenderer and a RigidBody3D.
This object can also be spawned in engine  */
class TestModel : public GameObject
{
public:

	MeshRenderer* mr;
	
	//! Base Test Model Constructor
	TestModel();
	//! Secondary Test Model Constructor
	/*! This is the main constructor that we will use and takes in a char* for our gameObject name 
	and a Vec3 Position for where our object will be in world space*/
	TestModel(const std::string& name, MATH::Vec3 position);

	//! Test Model Destructor
	~TestModel()
	{
		if(mr)
		{
			mr = nullptr;
		}
	}

	//! Attach Uniforms Function
	/*! This virtual function can be overridden to allow us to send custom variables to the shader*/
	virtual void AttachUniforms() const override;

	//! Update Function (Inherited from Gameobject)
	void Update(const float deltaTime) override {}

	//! Handle Events Function (Inherited from Gameobject)
	void HandleEvents(const SDL_Event& event) override {}
	 
	//! On Collision Enter  Function (Inherited from GameObject)
	/*! Is used to check what object is being collided with */
	virtual void OnCollisionEnter(Collider3D& otherBody) override {}



	std::shared_ptr<GameObject> NewClone() const override { return std::make_shared<TestModel>(this->name, this->transform.GetPosition()); }

private:

	

};

