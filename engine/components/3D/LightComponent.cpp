#include "LightComponent.h"


#include "Utility/SaveUtility.h"

#include "graphics/ShaderProgram.h"

#include "core/Globals.h"


#include "components/GameObject.h"
#include "components/SceneGraph.h"

using namespace MATH;

LightComponent::~LightComponent()
{
	Renderer::GetInstance()->DeleteLight(this);
}

void LightComponent::Init(GameObject* g)
{
	gameObject = g;
	lightInfo.type = LightType::POINT;
	lightInfo.ambColor = Vec3(0.0f);
	lightInfo.diffColor = Vec3(1.0f);
	lightInfo.specColor = Vec3(1.0f);
	lightInfo.intensity = 1.0f;
	lightInfo.cutOff = static_cast<float>(cos(12.5 * DEGREES_TO_RADIANS));
	lightInfo.outerCutOff = static_cast<float>(cos(15.0 * DEGREES_TO_RADIANS));

	lightInfo.attenConstant = 1.0f;
	lightInfo.attenLinear = 0.049f;
	lightInfo.attenQuadratic = 0.0f;
	
	Renderer::GetInstance()->AddLight(this);
}

void LightComponent::OnSaveComponent(const std::string& saveName, std::string parentName)
{

}

void LightComponent::ImGuiRender()
{
	const bool opened = Globals::Editor::OpenComponentTreeNode(this, "Light Settings");
	
	if (opened)
	{
		static int currentIndex = 0;
		if (ImGui::BeginCombo("LightTypes", lightTypeNameEnumPairs[static_cast<int>(lightInfo.type) - 1].typeName))
		{
			for (int i = 0; i < static_cast<int>(LightType::DIRECTIONAL); i++)
			{
				static bool isSelected = (currentIndex == i);

				if (ImGui::Selectable(lightTypeNameEnumPairs[i].typeName, isSelected))
				{
					currentIndex = i;
					lightInfo.type = static_cast<LightType>(i +1);
				}
			}
			ImGui::EndCombo();

		}

		ImGui::DragFloat("Intensity", &lightInfo.intensity, 0.1f, 0.0f, 500.0f);
		
		switch (lightInfo.type)
		{
			case LightType::POINT:
			{
				ImGui::DragFloat("Linear Attenuation", &lightInfo.attenLinear, 0.001f, 0.0f, 2.0f);
				ImGui::DragFloat("Quadratic Attenuation", &lightInfo.attenQuadratic, 0.001f, 0.0f, 3.0f);
				break;
			}
			case LightType::SPOT:
			{
				ImGui::DragFloat("Cut Off", &lightInfo.cutOff, 0.001f, lightInfo.outerCutOff, 1.0f);
				ImGui::DragFloat("Outer Cut Off", &lightInfo.outerCutOff, 0.001f, -1.0f, lightInfo.cutOff);
				break;
			}
			case LightType::DIRECTIONAL:
			{
			break;
			}
		}
		
		//ImGui::DragFloat3("Direction");
		ImGui::ColorEdit3("Ambient Color", lightInfo.ambColor);
		ImGui::ColorEdit3("Diffuse Color", lightInfo.diffColor);
		ImGui::ColorEdit3("Specular Color", lightInfo.specColor);

		ImGui::TreePop();
	}
}

void LightData::SendLightDataToShader(const ShaderProgram& shader, const Vec3& position, const Vec3& direct, const std::string& shaderString, bool isActive) const
{
	std::string arrayIndex = shaderString;
	std::string lightType = arrayIndex + "lightType";
	glUniform1i(glGetUniformLocation(shader.GetID(), lightType.c_str()), static_cast<int>(type));

	Vec3 lightPos;
	Vec3 lightAmb;
	Vec3 lightDiff;
	Vec3 lightSpec;
	Vec3 lightDir;

	float activeMultiplier = (isActive) ? 1.0f : 0.0f;

	switch (type)
	{
		case LightType::POINT:
		{
			std::string pos = arrayIndex + "lightPos";
			std::string amb = arrayIndex + "lightAmb";
			std::string diff = arrayIndex + "lightDiff";
			std::string spec = arrayIndex + "lightSpec";
			std::string intens = arrayIndex + "lightIntens";
			lightPos = position * activeMultiplier;
			lightAmb = ambColor * activeMultiplier;
			lightDiff = diffColor * activeMultiplier;
			lightSpec = specColor * activeMultiplier;
			
			glUniform3f(glGetUniformLocation(shader.GetID(), pos.c_str()), lightPos.x, lightPos.y, lightPos.z);
			glUniform4f(glGetUniformLocation(shader.GetID(), amb.c_str()), lightAmb.x, lightAmb.y, lightAmb.z, attenConstant);
			glUniform4f(glGetUniformLocation(shader.GetID(), diff.c_str()), lightDiff.x, lightDiff.y, lightDiff.z, attenLinear);
			glUniform4f(glGetUniformLocation(shader.GetID(), spec.c_str()), lightSpec.x, lightSpec.y, lightSpec.z, attenQuadratic);
			glUniform1f(glGetUniformLocation(shader.GetID(), intens.c_str()), intensity);
			break;
		}
		case LightType::SPOT:
		{
			std::string pos = arrayIndex + "lightPos";
			std::string dir = arrayIndex + "lightDir";
			std::string amb = arrayIndex + "lightAmb";
			std::string diff = arrayIndex + "lightDiff";
			std::string spec = arrayIndex + "lightSpec";
			std::string intens = arrayIndex + "lightIntens";
			lightPos = position * activeMultiplier;
			lightDir = direct * activeMultiplier;
			lightAmb = ambColor * activeMultiplier;
			lightDiff = diffColor * activeMultiplier;
			lightSpec = specColor * activeMultiplier;
			
			glUniform3f(glGetUniformLocation(shader.GetID(), pos.c_str()), lightPos.x, lightPos.y, lightPos.z);
			glUniform4f(glGetUniformLocation(shader.GetID(), dir.c_str()), lightDir.x, lightDir.y, lightDir.z, cutOff);
			glUniform4f(glGetUniformLocation(shader.GetID(), amb.c_str()), lightAmb.x, lightAmb.y, lightAmb.z, 0.0f);
			glUniform4f(glGetUniformLocation(shader.GetID(), diff.c_str()), lightDiff.x, lightDiff.y, lightDiff.z, 0.0f);
			glUniform4f(glGetUniformLocation(shader.GetID(), spec.c_str()), lightSpec.x, lightSpec.y, lightSpec.z, 0.0f);
			glUniform1f(glGetUniformLocation(shader.GetID(), intens.c_str()), outerCutOff);
			break;
		}
		case LightType::DIRECTIONAL:
		{
			std::string dir = arrayIndex + "lightDir";
			std::string amb = arrayIndex + "lightAmb";
			std::string diff = arrayIndex + "lightDiff";
			std::string spec = arrayIndex + "lightSpec";
			std::string intens = arrayIndex + "lightIntens";
			lightDir = direct * activeMultiplier;
			lightAmb = ambColor * activeMultiplier;
			lightDiff = diffColor * activeMultiplier;
			lightSpec = specColor * activeMultiplier;
			
			glUniform4f(glGetUniformLocation(shader.GetID(), dir.c_str()), lightDir.x, lightDir.y, lightDir.z, 0.0f);
			glUniform4f(glGetUniformLocation(shader.GetID(), amb.c_str()), lightAmb.x, lightAmb.y, lightAmb.z, 0.0f);
			glUniform4f(glGetUniformLocation(shader.GetID(), diff.c_str()), lightDiff.x, lightDiff.y, lightDiff.z, 0.0f);
			glUniform4f(glGetUniformLocation(shader.GetID(), spec.c_str()), lightSpec.x, lightSpec.y, lightSpec.z, 0.0f);
			glUniform1f(glGetUniformLocation(shader.GetID(), intens.c_str()), intensity);
			break;
		}
	}
}
