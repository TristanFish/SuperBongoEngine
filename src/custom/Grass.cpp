#include "Grass.h"

Grass::Grass()
{

}

Grass::Grass(const char* name, MATH::Vec3 position) : MeshRenderer("resources/models/LowPolyGrass.obj")
{
	this->name = name;
	transform.setPos(position);
	MeshRenderer::Init(this);
	MeshRenderer::CreateShader("src/graphics/shaders/GrassVert.glsl", "src/graphics/shaders/GrassFrag.glsl");
	MeshRenderer::SetInstanceID(1);
	amount = 750;
	MeshRenderer::SetInstanceAmount(amount);
	CalculateModelMatrices();
}

Grass::~Grass()
{
}

void Grass::Update(const float deltaTime)
{
	transform.Update(deltaTime);
	MeshRenderer::Update(deltaTime);
}

void Grass::Render() const
{
	MeshRenderer::Render();

	
}

void Grass::HandleEvents(const SDL_Event& event)
{
	MeshRenderer::HandleEvents(event);
}

void Grass::CalculateModelMatrices()
{
	MATH::Matrix4* modelMatrices;
	modelMatrices = new MATH::Matrix4[amount];
	float radius = 1.0;
    float offset = 50.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		MATH::Matrix4 model = MATH::Matrix4(1.0f);
		MATH::Vec3 NextPos = MATH::Vec3(0.0f, 0.0f, 0.0f);

		// Changes the values of the models that we want to load.
		float angle = float(i) / (float)amount * 360;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;

		NextPos = transform.pos + MATH::Vec3(x, 0.0f, z);
		
		model = MMath::translate(NextPos) * MMath::scale(MATH::Vec3(2.0f));
		modelMatrices[i] = model;
	}

	// THE BELOW CODE IS REQUIRED FOR INSTANCING 
	// Bind's the instance buffer and send's the model matricies to the shader
	unsigned int instanceBuffer;
	glGenBuffers(1, &instanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(MATH::Matrix4), &modelMatrices[0], GL_STATIC_DRAW);


	// Loops through all the meshes, bind's to a new buffer and set's their attributes.
	for (unsigned int i = 0; i < MeshRenderer::GetMeshes().size(); i++)
	{
		unsigned int VAO = MeshRenderer::GetMeshes()[i].GetVAO();

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
