#include "Instancer.h"
#include "math/MMath.h"

using namespace MATH;

Instancer::Instancer() : modelMatrices(nullptr)
{
}
Instancer::~Instancer()
{
	delete modelMatrices;
	glDeleteBuffers(1, &instanceBuffer);
}
void Instancer::Init(const unsigned int& amount_, GameObject* g)
{
	CalculateModelMatrices(g->transform, amount_);
	BindBuffers(g->getComponent<MeshRenderer>(), amount_);
}



void Instancer::CalculateModelMatrices(const Transform& transform, const unsigned int instanceAmount)
{
	modelMatrices = new MATH::Matrix4[instanceAmount];
	float radius = 1.0;
	float offset = 50.0f;
	for (unsigned int i = 0; i < instanceAmount; i++)
	{
		MATH::Matrix4 model = MATH::Matrix4(1.0f);
		MATH::Vec3 NextPos = MATH::Vec3(0.0f, 0.0f, 0.0f);
		// The Below Code/Math is from LearnOpengl.com(Instancing)
		// Changes the values of the models that we want to load.
		float angle = float(i) / (float)instanceAmount * 360;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;

		NextPos = transform.pos + MATH::Vec3(x, 0.0f, z);
		
		model = MMath::translate(NextPos) * MMath::calcRotationMatrix(transform.rotation) * MMath::scale(transform.scale);
		modelMatrices[i] = model;
	}
}

void Instancer::BindBuffers(const MeshRenderer& renderer, const unsigned int instanceAmount)
{
	// THE BELOW CODE IS REQUIRED FOR INSTANCING 
	// Binds the instance buffer and send's the model matricies to the shader

	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, instanceAmount * sizeof(MATH::Matrix4), &modelMatrices[0], GL_STATIC_DRAW);


	// Loops through all the meshes, binds to a new vertex array and sets their attributes.
	for (unsigned int i = 0; i < renderer.GetMeshes().size(); i++)
	{
		
		unsigned int VAO = renderer.GetMeshes()[i].GetVAO();

		glBindVertexArray(VAO);

		// Set attribute pointers for matrix (4 Vec4s)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(MATH::Matrix4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(MATH::Matrix4), (void*)(sizeof(MATH::Vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(MATH::Matrix4), (void*)(2 * sizeof(MATH::Vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(MATH::Matrix4), (void*)(3 * sizeof(MATH::Vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}
