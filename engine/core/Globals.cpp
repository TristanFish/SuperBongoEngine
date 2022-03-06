#include "Globals.h"

#include <memory>


#include "components/SceneGraph.h"
#include "core/CoreEngine.h"
#include "core/scene/Scene.h"
#include "components/GameObject.h"
#include "components/3D/MeshRenderer.h"
#include "core/resources/TextureManager.h"



std::string Globals::Engine::ENGINE_PATH = std::filesystem::current_path().string();
std::string Globals::Engine::SAVE_DATA_PATH = ENGINE_PATH + "\\resources\\SaveData\\";
std::string Globals::Engine::SCENE_NAME = "";

std::shared_ptr<SceneGraph> Globals::Engine::s_SceneGraph = nullptr;



std::shared_ptr<GameObject>Globals::Editor::selectedObj = nullptr;
int Globals::Editor::GizmoType = -1;

#pragma region Engine Globals


std::string Globals::Engine::IntToVectorWithPrefix(const char* typePrefix, const int i)
{
	
		std::string Prefix = std::string(typePrefix) + "_";
		switch (i)
		{
		case 0:
			return  Prefix + "X";
			break;

		case 1:
			return Prefix + "Y";
			break;
		case 2:
			return Prefix + "Z";
			break;
		case 3:
			return Prefix + "W";
			break;
		}

	return "ERROR";
}


std::string Globals::Engine::IntToVector(const int i)
{
	switch (i)
	{
	case 0:
		return  "X";
		break;

	case 1:
		return "Y";
		break;
	case 2:
		return "Z";
		break;
	case 3:
		return "W";
		break;
	}
}

SceneGraph* Globals::Engine::GetSceneGraph()
{
	return s_SceneGraph.get();
}

void Globals::Engine::InitGlobals()
{
	s_SceneGraph = std::shared_ptr(CoreEngine::GetInstance()->GetCurrentScene()->objectList);

	SCENE_NAME = CoreEngine::GetInstance()->GetCurrentScene()->GetSceneName();
}

#pragma endregion




#pragma region Editor Globals



bool Globals::Editor::DrawVec3(const std::string& label, MATH::Vec3& value, const float columnWidth)
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

void Globals::Editor::DrawTextureSlot(const char* textureName, MeshRenderer* meshRenderer, const float spacing, const MATH::Vec2& size)
{
	GLuint textureID = TextureManager::GetTexture(textureName).getTextureID();

	float textWidth = ImGui::CalcTextSize("Loaded Mesh").x;
	ImGui::SetCursorPos({ (ImGui::GetWindowSize().x - textWidth) / 2 ,ImGui::GetCursorPos().y });
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

	ImGui::SetCursorPos({ xTexPos, yTextPos + (offset * 2) });
	ImGui::Text(path.string().c_str());

	ImGui::Columns(1);

}

bool Globals::Editor::OpenComponentTreeNode(Component* comp, const char* name)
{
	const ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;

	const bool opened = ImGui::TreeNodeEx(name, tree_flags, name);

	std::string state = "active";

	if (!comp->active) state = "inactive";

	ImGui::SameLine(0, -1);

	if (ImGui::Button(state.c_str()))
	{
		comp->active = !comp->active;
	}

	return opened;
}

#pragma endregion