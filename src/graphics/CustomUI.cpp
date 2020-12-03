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



void CustomUI::PerformancePanel::Update(const float deltatime)
{
	fpsUpdateSpeed -= deltatime;

	if (fpsUpdateSpeed <= 0.0f)
	{
		if (fpsValues.size() == 100)
			fpsValues.clear();

		fpsValues.push_back(PerformanceMonitor::GetFPS());
		fpsUpdateSpeed = initSpeed;

		lastestFPS = (int)PerformanceMonitor::GetFPS();
	}
}

void CustomUI::PerformancePanel::Render() const
{
	ImGui::Begin("Performance");

	ImGui::PlotLines("FPS", fpsValues.data(), fpsValues.size()); 	ImGui::SameLine();
	ImGui::Text("%i", lastestFPS);


	ImGui::Checkbox("Limit FPS", &PerformanceMonitor::LimitFPS); ImGui::SameLine();
	ImGui::InputInt("", &PerformanceMonitor::FPSLimit);

	ImGui::End();
}

int PerformanceMonitor::FPSLimit = 60;

bool PerformanceMonitor::LimitFPS = false;

void PerformanceMonitor::DebugFPS()
{
	std::cout << "FPS: " << 1/Timer::GetDeltaTime()  << std::endl;
}

float PerformanceMonitor::GetFPS()
{
	return 1 / Timer::GetDeltaTime();
}

