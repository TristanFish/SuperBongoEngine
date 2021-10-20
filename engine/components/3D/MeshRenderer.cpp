#include "MeshRenderer.h"
#include "Rendering/Camera.h"
#include "core/resources/ModelManager.h"
#include "core/resources/ShaderManager.h"
#include "core/resources/TextureManager.h"
#include "graphics/UIStatics.h"

#include "Utility/SaveUtility.h"

using namespace MATH;

MeshRenderer::MeshRenderer() : renderFlags(RP_LIGHTING), meshColorTint(Vec4(1.0)), model(nullptr), instanceID(0)  { }

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

	if(model)
	{
		OBB.maxVert = model->p_max ;
		OBB.minVert = model->p_min ;
		OBB.transform = gameObject->transform.GetModelMatrix();
	}
}

void MeshRenderer::Update(const float deltaTime)
{
	if(model)
	{
		//assume uniform scale
		float scale = gameObject->transform.scale.x;
		//if(scale >= 1.0f)
		//{
		//	
		//} else
		//{
		//	scale /= 2.0f;
		//}
		
		OBB.maxVert = model->p_max * scale;
		OBB.minVert = model->p_min * scale;
		OBB.transform = gameObject->transform.GetModelMatrix();
	}
}

void MeshRenderer::Render() const
{
    const Matrix3 normMat = MMath::transpose(MMath::inverse(gameObject->GetModelMatrix()));

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
	const Matrix3 normMat = MMath::transpose(MMath::inverse(gameObject->GetModelMatrix()));
	shader.TakeUniform("meshColorTint", meshColorTint);
	shader.TakeUniform("projectionMatrix", Camera::getInstance()->getProjectionMatrix());
	shader.TakeUniform("viewMatrix", Camera::getInstance()->getViewMatrix());
	shader.TakeUniform("normalMatrix", normMat);
	shader.TakeUniform("modelMatrix", gameObject->GetModelMatrix());

	for (auto& m : model->meshes)
	{
		m.RenderRegular(shader);
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
	ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
	
	bool opened = ImGui::TreeNodeEx("MeshRenderer", tree_flags, "Renderer");

	if (opened)
	{
		ImGui::ColorEdit4("Mesh Color", meshColorTint);

		
		GLuint textureID = TextureManager::GetTexture("texture_09.jpg").getTextureID();


		if (ImGui::BeginCombo("##Flags", "Render Flags"))
		{

			const int renderFlagSize = IM_ARRAYSIZE(RenderFlagNameEnumPairs);


			for (size_t i = 0; i < renderFlagSize; i++)
			{
				bool boxIsActive = (renderFlags & RenderFlagNameEnumPairs[i].flagEnum);
				
				if (ImGui::Checkbox(RenderFlagNameEnumPairs[i].flagName, &boxIsActive))
				{
					std::cout << "Checkbox Open" << std::endl;
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


