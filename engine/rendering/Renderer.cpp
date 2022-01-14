#include "Renderer.h"
#include "components/3D/MeshRenderer.h"
#include "components/3D/LightComponent.h"
#include "core/Globals.h"
#include <sdl/SDL.h>
#include "Rendering/SkyBox.h"
#include "math/Plane.h"
#include "core/resources/ShaderManager.h"


std::unique_ptr<Renderer> Renderer::rendererInstance = std::unique_ptr<Renderer>();


using namespace MATH;


void Renderer::Init()
{

	skyBox = new SkyBox();

	//Set up renderer shaders
	gBufferShader = ShaderManager::GetShaders("gBufferShaderVert.glsl", "gBufferShaderFrag.glsl");
	resultShader = ShaderManager::GetShaders("gBufferResolveVert.glsl", "gBufferResolveFrag.glsl");

	SetupTextures();
	SetupFrameBuffers();

	vao = 0;
	
	float quadVertices[] =
	{	//positions							//uvs
		-1.0f,	-1.0f,	0.0f,		0.0f, 0.0f,
		-1.0f,	1.0f,	0.0f,		0.0f, 1.0f,
		1.0f,	-1.0f,	0.0f,		1.0f, 0.0f,
		1.0f,	1.0f,	0.0f,		1.0f, 1.0f,
	};
	//Setup vertex array for the result of the gBuffer
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	
	
}

void Renderer::SetupFrameBuffers()
{
	//Setup default buffer
	defaultBuffer.bufferID = 0;
	defaultBuffer.clearColor = Colour(1.0f, 0.0f, 0.0f, 1.0f);
	
	gBuffer.InitFrameBuffer();
	//Attach depthRenderBuffer
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
	gBuffer.AttachTexture(albedoTexture);
	gBuffer.AttachTexture(normTexture);
	gBuffer.AttachTexture(posTexture);
	gBuffer.AttachTexture(depthTexture);
	gBuffer.AttachTexture(stencilTexture);
	gBuffer.FinalizeBuffer();

	gBufferRenderResult.InitFrameBuffer();
	gBufferRenderResult.AttachTexture(gBufferTexture);
	gBufferRenderResult.FinalizeBuffer();
}

void Renderer::SetupTextures()
{
	albedoTexture = BufferTexture(BufferTexture::TexType::FOUR_COMP_SIGNED_COLOUR);
	normTexture = BufferTexture(BufferTexture::TexType::THREE_COMP_SIGNED_COLOUR);
	posTexture = BufferTexture(BufferTexture::TexType::THREE_COMP_SIGNED_COLOUR);
	depthTexture = BufferTexture(BufferTexture::TexType::ONE_COMP_SIGNED_COLOUR);
	stencilTexture = BufferTexture(BufferTexture::TexType::ONE_COMP_UNSIGNED_INT);
	gBufferTexture = BufferTexture(BufferTexture::TexType::FOUR_COMP_SIGNED_COLOUR);
}

void Renderer::AddMeshRenderer(MeshRenderer* mr)
{
	const auto mrIt = std::find(meshRenderers.begin(), meshRenderers.end(), mr);
	//if this meshrenderer doesn't already exist then add it
	if(mrIt == meshRenderers.end())
	{
		meshRenderers.emplace_back(mr);
	}
}

void Renderer::DeleteMeshRenderer(MeshRenderer* mr)
{
	for (std::vector<MeshRenderer*>::iterator iter = meshRenderers.begin(); iter != meshRenderers.end(); iter++)
	{
		if (*iter == mr)
		{
			meshRenderers.erase(iter);
			break;
		}
	}
}

void Renderer::AddLight(LightComponent* light)
{
	if(lights.size() >= 10)
	{
		EngineLogger::Warning("Max number of lights reached, " 
		 + light->gameObject->GetName() + " not added to the renderer", "Renderer.cpp", __LINE__, MessageTag::TYPE_GRAPHICS);
		return;
	}
	//if this meshrenderer doesn't already exist then add it
	const auto lightIt = std::find(lights.begin(), lights.end(), light);
	if(lightIt == lights.end())
	{
		lights.emplace_back(light);
	}
}

void Renderer::DeleteLight(LightComponent* light)
{
	for (std::vector<LightComponent*>::iterator iter = lights.begin(); iter != lights.end(); iter++)
	{
		if (*iter == light)
		{
			lights.erase(iter);
			break;
		}
	}
}

void Renderer::DrawDebugGeometry(const std::vector<GameObject*>& objects)
{
	#ifdef _DEBUG
	for (auto* g : objects)
	{
		g->DrawDebugGeometry();
	}
#endif // DEBUG
}

void Renderer::Render() 
{
	//Skybox here
	skyBox->Render();

	DrawDebugGeometry(Globals::s_SceneGraph->GetGameObjects());

	//loop through all meshrenderers
	for (size_t i = 0; i < meshRenderers.size(); i++)
	{

		//if(!IsMeshOnScreen(*meshRenderers[i]))
		{
			//EngineLogger::Info(meshRenderers[i]->gameObject->GetName() + " was frustum culled", "Renderer.cpp", __LINE__);
			//continue;
		}

		//Check meshrenderers for specific flags and do certain functions based on those flags
		if (meshRenderers[i]->renderFlags & RenderProperties::RP_OVERRIDE_RENDERER)
		{
			meshRenderers[i]->Render();
			continue;
		}

		gBufferShader.RunShader();

	/*	LIGHTING			= 0b00000001,
		CREATES_SHADOWS		= 0b00000010,
		RECIEVES_SHADOWS	= 0b00000100,
		BLOOM				= 0b00001000,
		PHYSICS_MOVEMENT	= 0b00010000,
		TRANSPARENT			= 0b00100000,
		WATER				= 0b01000000,
		OVERRIDE_SHADER		= 0b10000000*/

		const Uint16 stencilMarker = meshRenderers[i]->renderFlags;

		if (meshRenderers[i]->renderFlags & RenderProperties::RP_CREATES_SHADOWS)
		{
			RenderShadowTexture(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::RP_RECIEVES_SHADOWS)
		{
			RenderShade(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::RP_BLOOM)
		{
			RenderBloom(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::RP_PHYSICS_MOVEMENT)
		{
			RenderPhysics(*meshRenderers[i]);
		}
		if (meshRenderers[i]->renderFlags & RenderProperties::RP_WATER)
		{
			RenderWaterEffects(*meshRenderers[i]);
		}

		gBufferShader.TakeUniform("stencilMarker", stencilMarker);
		
		meshRenderers[i]->Render(gBufferShader);
		glUseProgram(0);
	}
	
	
	//Rebind the default framebuffer
	defaultBuffer.Bind();
	glEnable(GL_DEPTH_TEST);

	RenderGBufferResult();

	//Uses the gBufferResolve shader to render the result of the gBuffer


}

void Renderer::DestroyRenderer()
{
	delete(skyBox);
	skyBox = nullptr;

	
	glDeleteRenderbuffers(1, &depthRenderBuffer);

	gBuffer.DeleteFramebuffer();
	gBufferRenderResult.DeleteFramebuffer();

	
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

Renderer* Renderer::GetInstance()
{
	if (rendererInstance == nullptr)
	{
		rendererInstance.reset(new Renderer);
	}
	return rendererInstance.get();
}

void Renderer::ClearComponents()
{
	meshRenderers.clear();
	lights.clear();
}

SkyBox* Renderer::GetSkyBox()
{
	 return skyBox; 
}


GLuint Renderer::GetModeTextureID() const
{
	switch (viewport.GetRenderMode())
	{
	case CustomUI::RenderMode::Result:
		return gBufferTexture.texture;
		break;
	case CustomUI::RenderMode::Albedo:
		return albedoTexture.texture;
		break;
	case CustomUI::RenderMode::Position:
		return posTexture.texture;
		break;
	case CustomUI::RenderMode::Normals:
		return normTexture.texture;
		break;
	case CustomUI::RenderMode::Depth:
		return depthTexture.texture;
		break;
	case CustomUI::RenderMode::Stencil:
		return stencilTexture.texture;
		break;
	default:
		break;
	}

	return albedoTexture.texture;
}

void Renderer::Resize(const int size_x, const int size_y)
{

	glDeleteRenderbuffers(1, &depthRenderBuffer);

	gBuffer.DeleteFramebuffer();
	gBufferRenderResult.DeleteFramebuffer();


	SetupFrameBuffers();
	
	//Set frame buffer back to default
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

bool Renderer::IsMeshOnScreen(const MeshRenderer& mr)
{
	//OrientedBoundingBox obb = mr.OBB;

	Camera* cam = Camera::getInstance();
	Matrix4 projViewMatrix = cam->getProjectionMatrix() * cam->getViewMatrix();


	Vec3 PVMPos;// = projViewMatrix * obb.transform.getColumn(3);

	//Get frustum planes
	Plane frustumPlanes[6];

	

	///	0  4  8  12
	///	1  5  9  13
	///	2  6  10 14
	///	3  7  11 15

	// 11  12  13  14
	// 21  22  23  24
	// 31  32  33  34
	// 41  42  43  44
	
	//Left & Right
	frustumPlanes[0] = Plane(projViewMatrix[3] + projViewMatrix[0], 
							 projViewMatrix[7] + projViewMatrix[4], 
							 projViewMatrix[11] + projViewMatrix[8], 
							 projViewMatrix[15] + projViewMatrix[12]);
	frustumPlanes[1] = Plane(projViewMatrix[3] - projViewMatrix[0], 
							 projViewMatrix[7] - projViewMatrix[4], 
							 projViewMatrix[11] - projViewMatrix[8], 
							 projViewMatrix[15] - projViewMatrix[12]);

	//Bottom & Top
	frustumPlanes[2] = Plane(projViewMatrix[3] + projViewMatrix[1], 
							 projViewMatrix[7] + projViewMatrix[5], 
							 projViewMatrix[11] + projViewMatrix[9], 
							 projViewMatrix[15] + projViewMatrix[13]);
	frustumPlanes[3] = Plane(projViewMatrix[3] - projViewMatrix[1], 
							 projViewMatrix[7] - projViewMatrix[5], 
							 projViewMatrix[11] - projViewMatrix[9], 
							 projViewMatrix[15] - projViewMatrix[13]);

	//Near & Far
	frustumPlanes[4] = Plane(projViewMatrix[3] + projViewMatrix[2], 
							 projViewMatrix[7] + projViewMatrix[6], 
							 projViewMatrix[11] + projViewMatrix[10], 
							 projViewMatrix[15] + projViewMatrix[14]);
	frustumPlanes[5] = Plane(projViewMatrix[3] - projViewMatrix[2], 
							 projViewMatrix[7] - projViewMatrix[6], 
							 projViewMatrix[11] - projViewMatrix[10], 
							 projViewMatrix[15] - projViewMatrix[14]);
	frustumPlanes[0].Normalize();
	frustumPlanes[1].Normalize();
	frustumPlanes[2].Normalize();
	frustumPlanes[3].Normalize();
	frustumPlanes[4].Normalize();
	frustumPlanes[5].Normalize();
	
	bool isVertexInsideAllPlanes = true;

	//loop foreach plane
	for(int j = 0; j < 6; j++)
	{
		//if any vertex is outside any plane this vertex cannot be inside the frustum
		if(frustumPlanes[j].DistanceFromPoint(PVMPos) < 0)
		{
			isVertexInsideAllPlanes = false;
		}

		//if this vertex is outside of any planes stop checking planes and move onto the next vertex
		if(isVertexInsideAllPlanes == false)
		{
			break;
		}
	}
	
	//if any vertex is inside all planes then this mesh should be rendered
	if(isVertexInsideAllPlanes == true)
	{
		return true;
	}


	return false;
}

//Binds all the gBuffer textures
void Renderer::BindGBufferTextures() const
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(resultShader.GetID(), "albedoTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, albedoTexture.texture);

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(resultShader.GetID(), "normTexture"), 1);
	glBindTexture(GL_TEXTURE_2D, normTexture.texture);

	glActiveTexture(GL_TEXTURE2);
	glUniform1i(glGetUniformLocation(resultShader.GetID(), "posTexture"), 2);
	glBindTexture(GL_TEXTURE_2D, posTexture.texture);

	glActiveTexture(GL_TEXTURE3);
	glUniform1i(glGetUniformLocation(resultShader.GetID(), "depthTexture"), 3);
	glBindTexture(GL_TEXTURE_2D, depthTexture.texture);

	glActiveTexture(GL_TEXTURE4);
	glUniform1i(glGetUniformLocation(resultShader.GetID(), "stencilTexture"), 4);
	glBindTexture(GL_TEXTURE_2D, stencilTexture.texture);
}

void Renderer::UnbindGBufferTextures() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::RenderGBufferResult() 
{
	gBufferRenderResult.Bind();
	resultShader.RunShader();

	resultShader.TakeUniform("camPos", Camera::getInstance()->getPosition());
	BindGBufferTextures();

	AttachLights();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	UnbindGBufferTextures();
	glUseProgram(0);

	defaultBuffer.Clear();

	viewport.Render();
}

void Renderer::AttachLights() const
{
	std::vector<Vec3> positions;
	std::vector<Vec3> ambs;
	std::vector<Vec3> diffs;
	std::vector<Vec3> specs;
	std::vector<float> intensities;

	positions.reserve(lights.size());
	ambs.reserve(lights.size());
	diffs.reserve(lights.size());
	specs.reserve(lights.size());
	intensities.reserve(lights.size());
	
	for(size_t i = 0; i < lights.size(); i++)
	{
		positions.push_back(lights[i]->gameObject->transform.GetPosition());
		ambs.push_back(lights[i]->ambColor);
		diffs.push_back(lights[i]->diffColor);
		specs.push_back(lights[i]->specColor);
		intensities.push_back(lights[i]->intensity);
	}
	
	resultShader.TakeUniform("activeLights", static_cast<uint16_t>(lights.size()));
	glUniform3fv(glGetUniformLocation(resultShader.GetID(), "lightsPos"), lights.size(), *positions.data());
	glUniform3fv(glGetUniformLocation(resultShader.GetID(), "lightsAmb"), lights.size(), *ambs.data());
	glUniform3fv(glGetUniformLocation(resultShader.GetID(), "lightsDiff"), lights.size(), *diffs.data());
	glUniform3fv(glGetUniformLocation(resultShader.GetID(), "lightsSpec"), lights.size(), *specs.data());
	glUniform1fv(glGetUniformLocation(resultShader.GetID(), "lightsIntens"), lights.size(), intensities.data());

	
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

void Renderer::RenderWaterEffects(const MeshRenderer& mr) const
{

}
