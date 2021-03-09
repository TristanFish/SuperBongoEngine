#include "MeshRenderer.h"
#include "custom/Camera.h"
#include "core/ModelManager.h"
#include "core/ShaderManager.h"

using namespace MATH;

MeshRenderer::MeshRenderer() : renderFlags(RenderProperties::LIGHTING), meshColorTint(Vec4(1.0)), model(nullptr), instanceID(0)  { }

bool MeshRenderer::LoadModel(const char* name)
{
	model = &ModelManager::GetModel(name);

	if(model)
	{
		return true;
	}
	return false;
}

void MeshRenderer::CreateShader(const char* vert, const char* frag)
{
	shader = ShaderManager::GetShaders(vert, frag);
}

void MeshRenderer::Init(GameObject* g)
{
	gameobject = g;
}

void MeshRenderer::Update(const float deltaTime) { }

void MeshRenderer::Render() const
{
    const Matrix3 normMat = MMath::transpose(MMath::inverse(gameobject->GetModelMatrix()));

	if(instanceID == 0)
    {
        shader.RunShader();
		shader.TakeUniform("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
		shader.TakeUniform("viewMatrix", Camera::getInstance()->getViewMatrix());
		shader.TakeUniform("modelMatrix", gameobject->GetModelMatrix());
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
		shader.TakeUniform("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
		shader.TakeUniform("viewMatrix", Camera::getInstance()->getViewMatrix());
		shader.TakeUniform("normalMatrix", normMat);
		AttachUniforms();
		model->meshes[0].RenderInstanced(shader, model->meshes, instanceAmount);
		glUseProgram(0);
    }

}

void MeshRenderer::Render(const ShaderProgram& shader) const
{
	const Matrix3 normMat = MMath::transpose(MMath::inverse(gameobject->GetModelMatrix()));
	shader.TakeUniform("meshColorTint", meshColorTint);
	shader.TakeUniform("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
	shader.TakeUniform("viewMatrix", Camera::getInstance()->getViewMatrix());
	shader.TakeUniform("normalMatrix", normMat);
	shader.TakeUniform("modelMatrix", gameobject->GetModelMatrix());

	for (auto& m : model->meshes)
	{
		m.RenderRegular(shader);
	}

}

const char* MeshRenderer::ComponentName() const
{
	return "MeshRenderer";
}


