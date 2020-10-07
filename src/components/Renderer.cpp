#include "Renderer.h"
#include "3D/MeshRenderer.h"
#include "3D/LightComponent.h"
#include "core/Globals.h"
#include "core/Debug.h"
#include <sdl/SDL.h>


void Renderer::Init()
{
	shader.CreateShader("src/graphics/shaders/gBufferShaderVert.glsl", "src/graphics/shaders/gBufferShaderFrag.glsl");
	
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	glBindTexture(GL_TEXTURE_2D, stencilTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_ATTACHMENT, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, GL_STENCIL_COMPONENTS, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilTexture, 0);

	glGenTextures(1, &posTexture);
	glBindTexture(GL_TEXTURE_2D, posTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTexture, 0);

	glGenTextures(1, &normTexture);
	glBindTexture(GL_TEXTURE_2D, normTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normTexture, 0);

	glGenTextures(1, &albedoTexture);
	glBindTexture(GL_TEXTURE_2D, albedoTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedoTexture, 0);

	const GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, buffers);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not complete!" << std::endl;
	}

	currentGTexture = albedoTexture;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

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
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

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
		glUseProgram(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Debug::DrawTextureToScreen(currentGTexture, -1.0f, -0.5f, -0.5f, -1.0f);
	pos.DrawTextureToScreen(posTexture, -1.0f, 0.0f, 0.0f, -1.0f);
	norm.DrawTextureToScreen(normTexture, -1.0f, 0.0f, 1.0f, 0.0f);
	albedo.DrawTextureToScreen(albedoTexture, 0.0f, 1.0f, 0.0f, -1.0f);
}

void Renderer::DestroyTextures()
{
	glDeleteRenderbuffers(1, &depthRenderBuffer);
	glDeleteTextures(1, &stencilTexture);
	glDeleteTextures(1, &posTexture);
	glDeleteTextures(1, &normTexture);
	glDeleteTextures(1, &albedoTexture);
	glDeleteFramebuffers(1, &gBuffer);

}

void Renderer::AttachLights(const MeshRenderer& mr) const
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		std::string lightNum = "uLights[" + std::to_string(i);
		
		shader.TakeInUniformVec3(lightNum + "].lPos", lights[i]->gameobject->transform.GetPosition());
		shader.TakeInUniformVec3(lightNum + "].lAmb", lights[i]->ambColor);
		shader.TakeInUniformVec3(lightNum + "].lDiff", lights[i]->diffColor);
		shader.TakeInUniformVec3(lightNum + "].lSpec", lights[i]->specColor);
		shader.TakeInUniformFloat(lightNum + "].lIntens", lights[i]->intensity);
		//shader.TakeInUniformInt("activeLights", lights.size());
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
