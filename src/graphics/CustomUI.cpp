#include "CustomUI.h"

CustomUI::PropertiesPanel::PropertiesPanel(GameObject* obj)
{
	selectedObj = obj;
}

CustomUI::PropertiesPanel::~PropertiesPanel()
{
	delete selectedObj;
	selectedObj = nullptr;
}

void CustomUI::PropertiesPanel::Render()
{
	if (selectedObj->isMenuActive)
	{
		// Gets the mesh's properties and then displays them with ImGui

		ImGui::Begin(selectedObj->name, &selectedObj->isMenuActive);

		char* TempName = new char();
		TempName = (char*)selectedObj->name;
		if (ImGui::InputText("Mesh Name", TempName, size_t(20)))
		{
			selectedObj->SetName(TempName);
		}

		// Change the standard transform components 
		ImGui::DragFloat3("Position", selectedObj->transform.GetPosition());
		ImGui::DragFloat3("Rotation", selectedObj->transform.GetRotation());
		ImGui::DragFloat3("Scale", selectedObj->transform.GetScale(), 0.0f, 10.0f);

		// Create a new color that is a copy of the meshes color
		ImGui::ColorEdit4("Mesh Color", (float*)selectedObj->getComponent<MeshRenderer>().meshColorTint);
		
		
		// MAKE SAVE MAP DATA A STATIC FUNCTION
		if (ImGui::Button("Save"))
		{
		
		}

		ImGui::End();
	}

}
