#include "Instancer.h"
#include "math/MMath.h"


#include "components/GameObject.h"

using namespace MATH;

Instancer::~Instancer()
{
	modelMatrices.clear();
	glDeleteBuffers(1, &instanceBuffer);
}

void Instancer::Init(std::shared_ptr<GameObject> g)
{
	gameObject = g;
	MeshRenderer* mr = g->GetComponent<MeshRenderer>();

	if(mr == nullptr)
	{
		EngineLogger::Warning("Gameobject: " + gameObject->GetName() + " has no meshrenderer for its instancer, instancer will not initialize", "Instancer.cpp", __LINE__);
		return;
	}

	const unsigned int amount = mr->GetInstanceAmount();
	
	CalculateModelMatrices(g->transform, amount);
	BindBuffers(*mr, amount);
}

void Instancer::Update(const float deltaTime)
{
}

void Instancer::CalculateModelMatrices(const Transform& transform, const unsigned int instanceAmount)
{
	modelMatrices.reserve(instanceAmount);
	const float radius = 1.0;
	const float offset = 50.0f;
	for (unsigned int i = 0; i < instanceAmount; i++)
	{
		Matrix4 model = Matrix4(1.0f);
		Vec3 NextPos = Vec3(0.0f, 0.0f, 0.0f);
		// The Below Code/Math is from LearnOpengl.com(Instancing)
		// Changes the values of the models that we want to load.
		const float angle = static_cast<float>(i) / static_cast<float>(instanceAmount) * 360.0f;
		float displacement = (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
		const float x = sin(angle) * radius + displacement;
		displacement = (rand() % static_cast<int>(2 * offset * 100)) / 100.0f - offset;
		const float z = cos(angle) * radius + displacement;

		NextPos = transform.GetPosition() + Vec3(x, 0.0f, z);
		
		model = MMath::translate(NextPos) * transform.GetRotationMatrix() * MMath::scale(transform.GetScale());
		modelMatrices.emplace_back(model);
	}
}

void Instancer::BindBuffers(const MeshRenderer& renderer, const unsigned int instanceAmount)
{
	// THE BELOW CODE IS REQUIRED FOR INSTANCING 
	// Binds the instance buffer and send's the model matricies to the shader

	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, instanceAmount * sizeof(Matrix4), modelMatrices.data(), GL_STATIC_DRAW);


	// Loops through all the meshes, binds to a new vertex array and sets their attributes.
	for (unsigned int i = 0; i < renderer.GetMeshes().size(); i++)
	{
		const unsigned int VAO = renderer.GetMeshes()[i].GetVAO();

		glBindVertexArray(VAO);

		// Set attribute pointers for matrix (4 Vec4s)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4), (void*)(sizeof(Vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4), (void*)(2 * sizeof(Vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix4), (void*)(3 * sizeof(Vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}
