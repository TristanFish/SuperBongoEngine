#include "UIStatics.h"

#include "components/GameObject.h"
#include "components/SceneGraph.h"

#include "core/CoreEngine.h"
#include "scenes/Scene.h"

#include "core/resources/TextureManager.h"

#include "imgui/imgui_internal.h"

#include "components/3D/MeshRenderer.h"

GameObject* UIStatics::selectedObj = nullptr;




UIStatics::~UIStatics()
{

}



void UIStatics::DrawVec3(const std::string label, MATH::Vec3& value, const float columnWidth)
{


	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();


	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 1.0f,1.0f });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.1f,0.1f,1.0f });
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::Button("X", buttonSize);
	ImGui::PopItemFlag();
	ImGui::PopStyleColor();


	ImGui::SameLine();
	ImGui::DragFloat("##X", &value.x, 0.1);
	ImGui::PopItemWidth();
	ImGui::SameLine();


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.8f,0.1f,1.0f });
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::Button("Y", buttonSize);
	ImGui::PopItemFlag();
	ImGui::PopStyleColor();


	ImGui::SameLine();
	ImGui::DragFloat("##Y", &value.y, 0.1);
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.1f,0.8f,1.0f });
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::Button("Z", buttonSize);
	ImGui::PopItemFlag();
	ImGui::PopStyleColor();

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &value.z, 0.1);
	ImGui::PopItemWidth();


	ImGui::Columns(1);
	ImGui::PopStyleVar();
	ImGui::PopID();
}

void UIStatics::DrawTextureSlot(const char* textureName, MeshRenderer* meshRenderer,const float spacing, const MATH::Vec2& size)
{
	GLuint textureID =  TextureManager::GetTexture(textureName).getTextureID();

	float textWidth = ImGui::CalcTextSize("Loaded Mesh").x;
	ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - textWidth) /2 ,ImGui::GetCursorPos().y });
	ImGui::Text("Loaded Mesh");


	ImGui::Columns(2, "TextureColumns", false);
	ImGui::SetColumnWidth(0, size.x * 1.25);



	ImGui::Dummy(ImVec2{ 0.0f,spacing });

	ImGui::ImageButton((ImTextureID)textureID, ImVec2{ size.x,size.y });

	if (ImGui::BeginDragDropTarget())
	{

		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content_Browser_Model"))
		{

			const char* name = (const char*)payload->Data;

			if (meshRenderer == nullptr)
				return;

			meshRenderer->LoadModel(name);
		}


		ImGui::EndDragDropTarget();
	}
	ImGui::NextColumn();


	


	ImGui::SameLine(0.0f, 25.0f);

	float offset = (size.x - ImGui::GetFont()->FontSize) / 4;
	float yTextPos = ImGui::GetCursorPos().y + offset;
	float xTexPos = ImGui::GetCursorPos().x;


	ImGui::SetCursorPos({ xTexPos,yTextPos });
	std::filesystem::path path = meshRenderer->GetModel()->modelPath;
	ImGui::Text(path.stem().string().c_str());


	ImGui::SetCursorPos({ xTexPos, yTextPos + (offset*2) });
	ImGui::Text(path.string().c_str());



	ImGui::Columns(1);

}


