#include "LightComponent.h"

#include "graphics/UIStatics.h"

using namespace MATH;


LightComponent::~LightComponent()
{
	Renderer::GetInstance()->DeleteLight(this);
}

void LightComponent::Init(GameObject* g)
{
	gameObject = g;
	type = LightType::POINT;
	ambColor = Vec3(0.0f);
	diffColor = Vec3(1.0f);
	specColor = Vec3(1.0f);
	intensity = 1.0f;
	cutOff = static_cast<float>(cos(12.5 * DEGREES_TO_RADIANS));
	outerCutOff = static_cast<float>(cos(15.0 * DEGREES_TO_RADIANS));

	attenConstant = 1.0f;
	attenLinear = 0.049f;
	attenQuadratic = 0.0f;
	
	Renderer::GetInstance()->AddLight(this);
}

void LightComponent::OnSaveComponent(const std::string& saveName, std::string parentName)
{

}

void LightComponent::ImGuiRender()
{
	ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
	
	bool opened = ImGui::TreeNodeEx("LightSettings", tree_flags, "Light Settings");

	if (opened)
	{

		static int currentIndex = 0;
		if (ImGui::BeginCombo("LightTypes", lightTypeNameEnumPairs[static_cast<int>(type) - 1].typeName))
		{
			for (int i = 0; i < static_cast<int>(LightType::DIRECTIONAL); i++)
			{

				static bool isSelected = (currentIndex == i);

				if (ImGui::Selectable(lightTypeNameEnumPairs[i].typeName, isSelected))
				{
					currentIndex = i;
					type = static_cast<LightType>(i +1);
				}
			}
			ImGui::EndCombo();

		}

		ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 500.0f);
		
		switch (type)
		{
			case LightType::POINT:
			{
				ImGui::DragFloat("Linear Attenuation", &attenLinear, 0.001f, 0.0f, 2.0f);
				ImGui::DragFloat("Quadratic Attenuation", &attenQuadratic, 0.001f, 0.0f, 3.0f);
				break;
			}
			case LightType::SPOT:
			{
				ImGui::DragFloat("Cut Off", &cutOff, 0.001f, outerCutOff, 1.0f);
				ImGui::DragFloat("Outer Cut Off", &outerCutOff, 0.001f, -1.0f, cutOff);
				break;
			}
			case LightType::DIRECTIONAL:
			{
			break;
			}
		}
		
		//ImGui::DragFloat3("Direction");
		ImGui::ColorEdit3("Ambient Color", ambColor);
		ImGui::ColorEdit3("Diffuse Color", diffColor);
		ImGui::ColorEdit3("Specular Color", specColor);

		ImGui::TreePop();
	}
}
