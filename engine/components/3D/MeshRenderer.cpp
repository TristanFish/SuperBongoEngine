#include "MeshRenderer.h"
#include "Rendering/Camera.h"
#include "core/resources/ModelManager.h"
#include "core/resources/ShaderManager.h"
#include "core/resources/TextureManager.h"
#include "core/3D/Physics//BoundingBox.h"
#include "graphics/UIStatics.h"

#include "Utility/SaveUtility.h"

using namespace MATH;

MeshRenderer::MeshRenderer() : renderFlags(RP_LIGHTING), meshColorTint(Vec4(1.0)), model(nullptr), instanceID(0)  { }

MeshRenderer::~MeshRenderer()
{
	Renderer::GetInstance()->DeleteMeshRenderer(this);
}

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
	gameObject = g;

	Renderer::GetInstance()->AddMeshRenderer(this);
}

void MeshRenderer::Update(const float deltaTime)
{
	if(model)
	{
		//assume uniform scale
		float scale = gameObject->transform.GetScale().x;
		//if(scale >= 1.0f)
		//{
		//	
		//} else
		//{
		//	scale /= 2.0f;
		//}
		
		//OBB.maxVert = model->p_max * scale;
		//OBB.minVert = model->p_min * scale;
		//OBB.transform = gameObject->transform.GetModelMatrix();
	}
}

void MeshRenderer::Render() const
{
    const Matrix3 normMat = gameObject->transform.GetRotationQuat().ConvertToMatrix();

	if(instanceID == 0)
    {
        shader.RunShader();
		shader.TakeUniform("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
		shader.TakeUniform("viewMatrix", Camera::getInstance()->getViewMatrix());
		shader.TakeUniform("modelMatrix", gameObject->GetModelMatrix());
        //shader.TakeInUniformMat3("normalMatrix", normMat);
		AttachUniforms();

		for (auto& m : model->meshes)
		{
			RenderMesh(m, shader);
		}
        glUseProgram(0);
    }

	if (instanceID >= 1)
	{
		shader.RunShader();
		shader.TakeUniform("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
		shader.TakeUniform("viewMatrix", Camera::getInstance()->getViewMatrix());
		shader.TakeUniform("normalMatrix", normMat);
		AttachUniforms();
		RenderInstancedMesh(model->meshes, shader, instanceAmount);
		glUseProgram(0);
    }
}

void MeshRenderer::Render(const ShaderProgram& shader_) const
{
	const Matrix3 normMat = gameObject->transform.GetRotationQuat().ConvertToMatrix();
	shader_.TakeUniform("meshColorTint", meshColorTint);
	shader_.TakeUniform("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
	shader_.TakeUniform("viewMatrix", Camera::getInstance()->getViewMatrix());
	shader_.TakeUniform("normalMatrix", normMat);
	shader_.TakeUniform("modelMatrix", gameObject->GetModelMatrix());

	for (auto& m : model->meshes)
	{
		RenderMesh(m, shader_);
	}
}

void MeshRenderer::RenderMesh(const Mesh& mesh, const ShaderProgram& shader) const
{
	size_t diffN = 1;
	size_t specN = 1;


	//For every texture on the mesh bind it to the shader
	for (size_t i = 0; i < mesh.textures.size(); i++)
	{
		//setting up a naming scheme for textures inside of shaders, check DefaultFrag.glsl
		//texture names should be "typeTexNumber" e.g.(diffuseTex1)
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = mesh.textures[i].type;
		if (name == "diffuseTex")
		{
			number = std::to_string(diffN++);
		}
		else if (name == "specularTex")
		{
			number = std::to_string(specN++);
		}


		glUniform1i(glGetUniformLocation(shader.GetID(), (name + number).c_str()), i);

		glBindTexture(GL_TEXTURE_2D, mesh.textures[i].getTextureID());
	}
	glActiveTexture(GL_TEXTURE0);
	
	if (diffN <= 1)
	{
		//if there are no diffuse textures, add the color of the material to the shader
		glUniform4fv(glGetUniformLocation(shader.GetID(), "meshColor"), 1, mesh.color);
	}
	else
	{
		glUniform4fv(glGetUniformLocation(shader.GetID(), "meshColor"), 1, Vec4());
	}

	glBindVertexArray(mesh.GetVAO());
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (size_t i = 0; i < mesh.textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

// Used for instance rendering 
void MeshRenderer::RenderInstancedMesh(const std::vector<Mesh>& meshes, const ShaderProgram& shader, unsigned int amount) const
{


	for(size_t meshCount = 0; meshCount < meshes.size(); meshCount++)
	{
		unsigned int diffN = 1;
		unsigned int specN = 1;
		//For every texture on the mesh bind it to the shader
		//This loop will probably start to get slow as we add more objects
		for (unsigned int textureCount = 0; textureCount < meshes[meshCount].textures.size(); textureCount++)
		{
			//setting up a naming scheme for textures inside of shaders, check DefaultFrag.glsl
			//texture names should be "typeTexNumber" e.g.(diffuseTex1)
			glActiveTexture(GL_TEXTURE0 + textureCount);

			std::string number;
			std::string name = meshes[meshCount].textures[textureCount].type;
			if (name == "diffuseTex")
			{
				number = std::to_string(diffN++);
			}
			else if (name == "specularTex")
			{
				number = std::to_string(specN++);
			}

			glUniform1i(glGetUniformLocation(shader.GetID(), (name + number).c_str()), textureCount);
			glBindTexture(GL_TEXTURE_2D, meshes[meshCount].textures[textureCount].getTextureID());
		}
		glActiveTexture(GL_TEXTURE0);

		if (diffN == 1)
		{
			//if there are no diffuse textures, add the color of the material to the shader
			glUniform4fv(glGetUniformLocation(shader.GetID(), "meshColor"), 1, meshes[meshCount].color);
		}

		
		glBindVertexArray(meshes[meshCount].GetVAO());
		glDrawElementsInstanced(GL_TRIANGLES, meshes[meshCount].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}
}

void MeshRenderer::OnSaveComponent(const std::string& saveName,std::string parentName)
{

	ElementInfo Renderer = ElementInfo(parentName);
	SaveUtility::GetInstance()->AddElement(saveName, "Renderer", Renderer);

	ElementInfo MeshTint = SaveUtility::GetInstance()->CreateVec4(meshColorTint, "Renderer");

	SaveUtility::GetInstance()->AddElement(saveName, "MeshColorTint", MeshTint);
}

void MeshRenderer::ImGuiRender()
{
	const bool opened = UIStatics::OpenComponentTreeNode(this, "MeshRenderer");
	
	if (opened)
	{
		ImGui::ColorEdit4("Mesh Color", meshColorTint);

		if (ImGui::BeginCombo("##Flags", "Render Flags"))
		{
			const int renderFlagSize = IM_ARRAYSIZE(RenderFlagNameEnumPairs);

			for (size_t i = 0; i < renderFlagSize; i++)
			{
				bool boxIsActive = (renderFlags & RenderFlagNameEnumPairs[i].flagEnum);
				
				if (ImGui::Checkbox(RenderFlagNameEnumPairs[i].flagName, &boxIsActive))
				{
					//If None is checked undo all boxes
					if(i == 0)
					{
						if(boxIsActive)
						{
							renderFlags = static_cast<RenderProperties>(0);
							break;
						}
					}
					
					if (renderFlags & RenderFlagNameEnumPairs[i].flagEnum)
					{
						renderFlags = static_cast<RenderProperties>(renderFlags & ~RenderFlagNameEnumPairs[i].flagEnum);
					} else
					{
						renderFlags = static_cast<RenderProperties>(renderFlags | RenderFlagNameEnumPairs[i].flagEnum);
					}
				}
			}
			ImGui::EndCombo();
		}
		UIStatics::DrawTextureSlot("texture_09.jpg", this,5.0f);

		ImGui::TreePop();
	}
}


