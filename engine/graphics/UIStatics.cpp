#include "UIStatics.h"

#include <memory>
#include <iostream>
#include <imgui/imgui_internal.h>

#include "components/GameObject.h"
#include "components/3D/MeshRenderer.h"
#include "core/resources/TextureManager.h"
#include "core/scene/Scene.h"

GameObject* UIStatics::selectedObj = nullptr;


bool UIStatics::DrawVec3(const std::string& label, MATH::Vec3& value, const float columnWidth)
{
	bool returnb = false;

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
	if (ImGui::DragFloat("##X", &value.x, 0.1f))
		returnb = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.8f,0.1f,1.0f });
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::Button("Y", buttonSize);
	ImGui::PopItemFlag();
	ImGui::PopStyleColor();


	ImGui::SameLine();
	if (ImGui::DragFloat("##Y", &value.y, 0.1f))
		returnb = true;
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.1f,0.8f,1.0f });
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
	ImGui::Button("Z", buttonSize);
	ImGui::PopItemFlag();
	ImGui::PopStyleColor();

	ImGui::SameLine();
	if (ImGui::DragFloat("##Z", &value.z, 0.1f))
		returnb = true;
	ImGui::PopItemWidth();


	ImGui::Columns(1);
	ImGui::PopStyleVar();
	ImGui::PopID();


	return returnb;
}

void UIStatics::DrawTextureSlot(const char* textureName, MeshRenderer* meshRenderer,const float spacing, const MATH::Vec2& size)
{
	GLuint textureID =  TextureManager::GetTexture(textureName).getTextureID();

	float textWidth = ImGui::CalcTextSize("Loaded Mesh").x;
	ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - textWidth) /2 ,ImGui::GetCursorPos().y });
	ImGui::Text("Loaded Mesh");


	ImGui::Columns(2, "TextureColumns", false);
	ImGui::SetColumnWidth(0, size.x * 1.25f);



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

void UIStatics::DrawSoundSlot(const char* textureName, AudioSourceComponent* audioSrc, const float spacing, const MATH::Vec2& size)
{
	GLuint textureID = TextureManager::GetTexture(textureName).getTextureID();

	float textWidth = ImGui::CalcTextSize("Audio Clip").x;
	ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - textWidth) / 2 ,ImGui::GetCursorPos().y });
	ImGui::Text("AudioClip");

	ImGui::Dummy(ImVec2{ 0.0f,spacing });

	ImGui::ImageButton((ImTextureID)textureID, ImVec2{ size.x,size.y });
	std::string soundName;
	if (ImGui::BeginDragDropTarget())
	{

		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content_Browser_Model"))
		{

			const char* name = (const char*)payload->Data;

			if (audioSrc == nullptr)
				return;
			soundName = name;
			audioSrc->LoadSound(name);
		}


		ImGui::EndDragDropTarget();
	}
	//ImGui::NextColumn();

	ImGui::SameLine(0.0f, 25.0f);

	float offset = (size.x - ImGui::GetFont()->FontSize) / 4;
	float yTextPos = ImGui::GetCursorPos().y + offset;
	float xTexPos = ImGui::GetCursorPos().x;

	ImGui::SetCursorPos({ xTexPos,yTextPos });
	//std::filesystem::path path = meshRenderer->GetModel()->modelPath;
	ImGui::Text(soundName.c_str());

	ImGui::SetCursorPos({ xTexPos, yTextPos + (offset * 2) });
	//ImGui::Text(path.string().c_str());

	ImGui::Columns(1);
}


bool UIStatics::OpenComponentTreeNode(Component* comp, const char* name)
{
	const ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
	
	const bool opened = ImGui::TreeNodeEx(name, tree_flags, name);

	std::string state = "active";

	if(!comp->active) state = "inactive";

	ImGui::SameLine(0, -1);
	
	if(ImGui::Button(state.c_str()))
	{
		comp->active = !comp->active;
	}
	
	return opened;
}


