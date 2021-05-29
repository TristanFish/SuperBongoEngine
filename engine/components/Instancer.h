#ifndef INSTANCER_H
#define INSTANCER_H
#include "components/ECS.h"
#include "components/Components.h"

//
//This class is going to be turned into a component
//

/*! Grass class
/*! This class handles how we are going to render multiple instances of a gameobject given only one model*/
class Instancer
{
	
public:

	//! Initialization function
	/*! This function calls all the needed functions for this class to work 
	- This function takes in an const unsigned int that is used for how many instances we want to spawn 
	- Also takes in a GameObject pointer is how we get the object's transform*/
	void Init(const unsigned int& amount_, GameObject* g);


	//! Base Instancer Constructor
	Instancer();
	//! Base Instancer Destructor
	~Instancer();
	

	//! Calculate Model Matrices function
	/*! This function does all the positional calculations for the instances. 
	- Takes in a object transform that allows us to get the objects model matrix 
	- Also takes in a unsigned int instancedAmount for how many instances we want*/
	virtual void CalculateModelMatrices(const Transform& transform, const unsigned int instanceAmount);
private:

	GLuint instanceBuffer;
	MATH::Matrix4* modelMatrices;

	//! Bind Buffers function
	/*!  Binds the instance buffer and sends the model matrices to the shader. 
	- Takes in a MeshRenderer to allow us to access the meshes vao buffer 
	- Also takes in unsigned int so the function knows how many to render*/
	void BindBuffers(const MeshRenderer& renderer, const unsigned int instanceAmount);

};

#endif

