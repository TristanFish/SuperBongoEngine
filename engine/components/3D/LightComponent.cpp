#include "LightComponent.h"

#include "graphics/UIStatics.h"

using namespace MATH;

void LightComponent::Init(GameObject* g)
{
	gameObject = g;
	type = LightType::POINT;
	ambColor = Vec3(1.0f);
	diffColor = Vec3(1.0f);
	specColor = Vec3(1.0f);
	intensity = 1.0f;
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
			for (int i = 0; i < static_cast<int>(LightType::SPOT); i++)
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
		ImGui::ColorEdit3("Ambient Color", ambColor);
		ImGui::ColorEdit3("Diffuse Color", diffColor);
		ImGui::ColorEdit3("Specular Color", specColor);

		ImGui::TreePop();
	}
}
