#include <assimp/Importer.hpp>
#include "MeshRenderer.h"
#include "custom/Camera.h"
#include "core/ModelManager.h"

MeshRenderer::MeshRenderer() : instanceID(0) { }

MeshRenderer::MeshRenderer(const char* name)
{
	renderFlags = RenderProperties::LIGHTING;
	model = &ModelManager::GetModel(name);
}

MeshRenderer::~MeshRenderer() { }

void MeshRenderer::CreateShader(const char* vert, const char* frag)
{
	shader.CreateShader(vert, frag);
}

void MeshRenderer::Init(GameObject* g)
{
	gameobject = g;
}

void MeshRenderer::Update(const float deltaTime) { }

void MeshRenderer::Render() const
{
    Matrix3 normMat = MMath::transpose(MMath::inverse(gameobject->GetModelMatrix()));

	if(instanceID == 0)
    {
        shader.RunShader();
		shader.TakeInUniformMat4("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
		shader.TakeInUniformMat4("viewMatrix", Camera::getInstance()->getViewMatrix());
		shader.TakeInUniformMat4("modelMatrix", gameobject->GetModelMatrix());
        //shader.TakeInUniformMat3("normalMatrix", normMat);
		AttachUniforms();

		for (auto& m : model->meshes)
		{
			m.RenderRegular(shader);
		}
        glUseProgram(0);
    }

	if (instanceID == 1)
	{
		shader.RunShader();
		shader.TakeInUniformMat4("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
		shader.TakeInUniformMat4("viewMatrix", Camera::getInstance()->getViewMatrix());
		shader.TakeInUniformMat3("normalMatrix", normMat);
		AttachUniforms();
		model->meshes[0].RenderInstanced(shader, model->meshes, instanceAmount);
		glUseProgram(0);
    }

}

void MeshRenderer::Render(const Shader& shader) const
{
	Matrix3 normMat = MMath::transpose(MMath::inverse(gameobject->GetModelMatrix()));
	shader.TakeInUniformVec4("meshColorTint", meshColorTint);
	shader.TakeInUniformMat4("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
	shader.TakeInUniformMat4("viewMatrix", Camera::getInstance()->getViewMatrix());
	shader.TakeInUniformMat3("normalMatrix", normMat);
	shader.TakeInUniformMat4("modelMatrix", gameobject->GetModelMatrix());

	for (auto& m : model->meshes)
	{
		m.RenderRegular(shader);
	}

}

void MeshRenderer::HandleEvents(const SDL_Event& event) { }


