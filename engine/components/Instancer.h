#ifndef INSTANCER_H
#define INSTANCER_H

#include "3D/MeshRenderer.h"
#include "glew/glew.h"

#include "Transform.h"

class GameObject;

/*! Instancer class
//! This class handles how we are going to render multiple instances of a gameObject given only one model*/
class Instancer : public Component
{
public:

	//! Base Instancer Constructor
	Instancer() = default;
	//! Base Instancer Destructor
	virtual ~Instancer();
	
	void Init(std::shared_ptr<GameObject> g) override;
	void Update(const float deltaTime) override;
	void HandleEvents(const SDL_Event& event) override {}

private:

	GLuint instanceBuffer;
	std::vector<MATH::Matrix4> modelMatrices;

	//! Calculate Model Matrices function
	/*! This function does all the positional calculations for the instances. 
	- Takes in a object transform that allows us to get the objects model matrix 
	- Also takes in a unsigned int instancedAmount for how many instances we want*/
	virtual void CalculateModelMatrices(const Transform& transform, const unsigned int instanceAmount);

	//! Bind Buffers function
	/*!  Binds the instance buffer and sends the model matrices to the shader. 
	- Takes in a MeshRenderer to allow us to access the meshes vao buffer 
	- Also takes in unsigned int so the function knows how many to render*/
	void BindBuffers(const MeshRenderer& renderer, const unsigned int instanceAmount);
};

#endif

