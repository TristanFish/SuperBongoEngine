#include "Renderer.h"
#include "3D/MeshRenderer.h"
#include "3D/LightComponent.h"


void Renderer::Init()
{
	shader.CreateShader("src/graphics/shaders/SuperShaderVert.glsl", "src/graphics/shaders/SuperShaderFrag.glsl");
}

void Renderer::AddMeshRenderer(MeshRenderer* mr)
{
	meshRenderers.emplace_back(mr);
}

void Renderer::AddLight(LightComponent* light)
{
	lights.emplace_back(light);
}

void Renderer::Render() const
{
	for (size_t i = 0; i < meshRenderers.size(); i++)
	{
		shader.RunShader();

		if (meshRenderers[i]->renderFlags & RenderProperties::LIGHTING)
		{
			AttachLights(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::CREATES_SHADOWS)
		{
			RenderShadowTexture(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::LIGHTING)
		{
			RenderShade(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::LIGHTING)
		{
			RenderBloom(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::LIGHTING)
		{
			RenderPhysics(*meshRenderers[i]);
		}
		meshRenderers[i]->Render(shader);
	}
}

void Renderer::AttachLights(const MeshRenderer& mr) const
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		std::string lightNum = "lights[" + std::to_string(i);
		
		shader.TakeInUniformVec3("lightPos[0]", lights[i]->gameobject->transform.GetPosition());
		shader.TakeInUniformVec3("lightAmb[0]", lights[i]->ambColor);
		shader.TakeInUniformVec3("lightDiff[0]", lights[i]->diffColor);
		shader.TakeInUniformVec3("lightSpec[0]", lights[i]->specColor);
		shader.TakeInUniformfloat("lightIntens[0]", lights[i]->intensity);
		shader.TakeInUniformfloat("activeLights", lights.size());
	}
}

void Renderer::RenderShadowTexture(const MeshRenderer& mr) const
{
}

void Renderer::RenderShade(const MeshRenderer& mr) const
{
}

void Renderer::RenderBloom(const MeshRenderer& mr) const
{
}

void Renderer::RenderPhysics(const MeshRenderer& mr) const
{
}
