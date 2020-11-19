#pragma once
#include "math/Vector.h"
#include "imgui/imgui.h"
#include "components/ECS.h"
#include "components/3D/MeshRenderer.h"


namespace CustomUI
{

	class PropertiesPanel
	{
	public:
		PropertiesPanel(GameObject* obj);

		~PropertiesPanel();

		void Render();
		GameObject* selectedObj;
	private:


	};
}

