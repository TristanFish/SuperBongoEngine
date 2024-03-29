#include "CustomUI.h"

#include <Windows.h>
#include <ImGuizmo/ImGuizmo.h>
#include  <algorithm>

#include "psapi.h"
#include "core/CoreEngine.h"
#include "core/GameInterface.h"
#include "core/Globals.h"
#include "core/Timer.h"
#include "core/events/InputManager.h"
#include "core/resources/SaveManager.h"
#include "core/resources/TextureManager.h"
#include "core/scene/DefaultScene.h"
#include "Utility/LoadUtility.h"
#include "components/GameObject.h"

using namespace CustomUI;

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#pragma region NetworkPanel

void NetworkPanel::SetNetworkRole(NetRole role)
{
	this->role = role;
	roleIsSet = true;
	NetworkManager::GetInstance()->role = role;

	NetworkManager::GetInstance()->CreateHost(7777, 4);	
}

void NetworkPanel::Disconnect()
{
	NetworkManager::GetInstance()->Disconnect();
	isConnected = false;
}

void NetworkPanel::Render()
{
	ImGui::Begin("Networking");

	if(!roleIsSet)
	{
		ImGui::Text("Select a network role");

		if(ImGui::Button("Server"))
		{
			SetNetworkRole(NetRole::SERVER);
		}
		ImGui::SameLine();
		if(ImGui::Button("Client"))
		{
			SetNetworkRole(NetRole::CLIENT);
		}		
	}
	else if(!isConnected && role == NetRole::CLIENT)
	{
		if(ImGui::Button("Connect"))
		{
			if(NetworkManager::GetInstance()->Connect("",7777))
			{
				isConnected = true;
			}
		}
	}
	else
	{
		if(role == NetRole::CLIENT)
		{
			if(ImGui::Button("Disconnect"))
			{
				Disconnect();
			}
		}

		std::string sendData;
		if(ImGui::InputText("Send Message", &sendData, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			NetworkManager::GetInstance()->SendPacket(sendData);
		}	
	}
	ImGui::End();
}



#pragma endregion 

#pragma region PropertiesPanel

PropertiesPanel::PropertiesPanel() : isActive(true)
{
}

PropertiesPanel::~PropertiesPanel()
{
}

void PropertiesPanel::Render() 
{
	std::shared_ptr<GameObject> selectedObject = Globals::Editor::GetSelectedObject();

	ImGui::Begin("Properties",&isActive);

	if (selectedObject)
	{
		#pragma region GameObject

		static std::string oldObjName = Globals::Editor::GetSelectedObject()->GetName();
		if (ImGui::InputText("Mesh Name", &Globals::Editor::GetSelectedObject()->GetNameRef(), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::string newObjName = Globals::Editor::GetSelectedObject()->GetName();

			SaveManager::GetSaveFile(Globals::Engine::SCENE_NAME).SetElementName(oldObjName, newObjName);
			SaveManager::SetSaveName(oldObjName, newObjName);

		}

		ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
		
		bool opened = ImGui::TreeNodeEx((void*)selectedObject.get(), tree_flags, "Transform");
		if (opened)
		{
			// Change the standard transform components 
			Globals::Editor::DrawVec3("Position", selectedObject->transform.GetPositionRef(), 80.0f);

			static MATH::Vec3 rotation = selectedObject->transform.GetRotation();

			{
				//clamp the euler angles
				if(rotation.x > 180.0f)
				{
					rotation.x -= 360.0f;
				}
				if(rotation.x < -180.0f)
				{
					rotation.x += 360.0f;
				}
				if(rotation.y > 180.0f)
				{
					rotation.y -= 360.0f;
				}
				if(rotation.y < -180.0f)
				{
					rotation.y += 360.0f;
				}
				if(rotation.z > 180.0f)
				{
					rotation.z -= 360.0f;
				}
				if(rotation.z < -180.0f)
				{
					rotation.z += 360.0f;
				}
			}

			if(Globals::Editor::DrawVec3("Rotation", rotation, 80.0f)){
			
				selectedObject->transform.SetRot(rotation);
			}
			
			Globals::Editor::DrawVec3("Scale", selectedObject->transform.GetScaleRef(), 80.0f);

			ImGui::TreePop();
		}
		
		selectedObject->ImguiRender();

		#pragma endregion 

		for(Component* comp : selectedObject->GetComponents())
		{
			comp->ImGuiRender();
		}
	}
	ImGui::End();
}

void CustomUI::PropertiesPanel::Reset()
{
	
}

#pragma endregion

#pragma region HierarchyPanel

HierarchyPanel::HierarchyPanel() : isActive(true) {}

HierarchyPanel::~HierarchyPanel()
{
	gameobjects.clear();
}

void HierarchyPanel::Construct()
{
	UpdateActiveObjects();
	isActive = true;
}

void HierarchyPanel::Render() 
{
	ImGui::Begin("Object Hierarchy", &isActive, ImGuiWindowFlags_NoTitleBar);
	
	textFilter.Draw("##Obj Filter");

	for (size_t i = 0; i < gameobjects.size(); i++)
	{
		if (gameobjects[i]->GetParent() == nullptr)
		{
			if (textFilter.PassFilter(gameobjects[i]->GetName().c_str()))
			{
				GenerateTree(gameobjects[i], i);
			}
		}
	}
	ImGui::End();
}

void HierarchyPanel::Reset()
{
	gameobjects.clear();
}

void HierarchyPanel::Update(const float deltatime)
{
	if(InputManager::GetInstance()->GetKey(SDLK_DELETE))
	{
		Globals::Engine::s_SceneGraph->DeleteGameObject(Globals::Editor::GetSelectedObject());
	}
	
	size_t size = Globals::Engine::s_SceneGraph->GetGameObjects().size();
	if (gameobjects.size() < size || gameobjects.size() > size)
	{
		UpdateActiveObjects();
	}
}

void HierarchyPanel::GenerateTree(std::shared_ptr<GameObject> go, int index) 
{
	ImGuiTreeNodeFlags tree_flags = ((Globals::Editor::GetSelectedObject() == go) ? ImGuiTreeNodeFlags_Selected : 0) |
		((go->GetChildCount() == 0) ?  ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen : 0) 
		| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (go->GetChildCount() > 0)
	{
		bool nodeOpened = ImGui::TreeNodeEx((void*)go->GetUUID(), tree_flags, go->GetName().c_str());
		
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("Obj_Index", &index, sizeof(int));

			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			ImGuiDragDropFlags target_flags = 0;
			//target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
			//target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Obj_Index", target_flags))
			{
				int objIndex = *(const int*)payload->Data;

				std::shared_ptr<GameObject> movedObj = gameobjects[objIndex];

				go->AddChild(movedObj);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (go->GetParent())
			{
				if (ImGui::MenuItem("UnParent"))
				{
					go->GetParent()->RemoveChild(go);
				}
			}

			if (ImGui::MenuItem("Delete"))
			{
				SaveManager::GetSaveFile(Globals::Engine::SCENE_NAME).RemoveElement(go->GetName());
				Globals::Engine::s_SceneGraph->DeleteGameObject(go);
				Globals::Editor::SetSelectedObject(nullptr);
			}
			ImGui::EndPopup();
		}

		if (ImGui::IsItemClicked())
		{
			Globals::Editor::SetSelectedObject(go);
			go->isObjectSelected = true;
		}
	
		if (nodeOpened)
		{
			for (auto& obj : go->GetChildren())
			{
				GenerateTree(obj, GetObjIndex(obj->GetName()));
			}
			ImGui::TreePop();
		}
	}
	else 
	{
		ImGuiDragDropFlags dragDrop_flags = ImGuiDragDropFlags_None;

		bool nodeOpened = ImGui::TreeNodeEx((void*)go->GetUUID(), tree_flags, go->GetName().c_str());

		if (ImGui::IsItemClicked())
		{
			Globals::Editor::SetSelectedObject(go);
			go->isObjectSelected = true;
		}

		if (ImGui::BeginDragDropSource(dragDrop_flags))
		{
			ImGui::SetDragDropPayload("Obj_Index", &index, sizeof(int));
			ImGui::Text("This is a drag and drop source");

			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			ImGuiDragDropFlags target_flags = 0;
			//target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
			//target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Obj_Index", target_flags))
			{
				int objIndex = *(const int*)payload->Data;

				std::shared_ptr<GameObject> movedObj = gameobjects[objIndex];

				if (movedObj->GetParent() != nullptr)
				{
					movedObj->GetParent()->RemoveChild(movedObj);
				}
				go->AddChild(movedObj);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (go->GetParent())
			{
				if (ImGui::MenuItem("UnParent"))
				{
					go->GetParent()->RemoveChild(go);
				}
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete"))
			{
				SaveManager::GetSaveFile(Globals::Engine::SCENE_NAME).RemoveElement(go->GetName());
				Globals::Engine::s_SceneGraph->DeleteGameObject(go);
				Globals::Editor::SetSelectedObject(nullptr);
			}
			ImGui::EndPopup();
		}
	}
}

void HierarchyPanel::UpdateActiveObjects()
{
	gameobjects.clear();
	
	for (const auto& obj : Globals::Engine::s_SceneGraph->GetGameObjects())
	{
		std::vector<std::shared_ptr<GameObject>>::iterator iter;
		
		iter = std::find(gameobjects.begin(), gameobjects.end(), obj);
		if (iter == gameobjects.end())
		{
			gameobjects.emplace_back(obj);
		}
	}
}

int HierarchyPanel::GetObjIndex(const std::string& objName) const
{
	for (size_t i = 0; i < gameobjects.size(); i++)
	{
		if (gameobjects[i]->GetName() == objName)
		{
			return i;
		}
	}

	return -1;
}

#pragma endregion

#pragma region PerformancePanel

float PerformanceMonitor::RenderLoopTime = 0.0f;
float PerformanceMonitor::UpdateLoopTime = 0.0f;

 
// Varibables that are used to collect CPU data
ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
int numProcessors;
HANDLE self;

PerformancePanel::PerformancePanel() : latestFPS(0) {}

PerformancePanel::~PerformancePanel() {}

void PerformancePanel::Update(const float deltatime)
{
	fpsUpdateSpeed -= deltatime;

	if (fpsUpdateSpeed <= 0.0f)
	{
		if (fpsValues.size() == 100)
			fpsValues.erase(fpsValues.begin());

		fpsValues.push_back(PerformanceMonitor::GetFPS());
		fpsUpdateSpeed = initSpeed;

		latestFPS = static_cast<int>(PerformanceMonitor::GetFPS());
	}
}

void PerformancePanel::Render()
{
	ImGui::Begin("Performance Monitor");

	ImGui::PlotLines("FPS", fpsValues.data(), fpsValues.size()); 	ImGui::SameLine();
	ImGui::Text("%i", latestFPS);

	ImGui::Checkbox("Limit FPS", &CoreEngine::GetInstance()->limitfps); ImGui::SameLine();
	ImGui::InputInt("##Input", reinterpret_cast<int*>(&CoreEngine::GetInstance()->fps));
	//ImGui::SliderInt("", reinterpret_cast<int*>(&CoreEngine::GetInstance()->fps), 0, 200);
	
	ImGui::Text("Render Loop Time %f ms", PerformanceMonitor::RenderLoopTime);
	ImGui::Text("Update Loop Time %f ms", PerformanceMonitor::UpdateLoopTime);
	ImGui::Text("Physical Memory Usage %i MB", PerformanceMonitor::GetMemoryUsage());
	ImGui::Text("CPU Usage %f %%", PerformanceMonitor::GetCPUUsage());

	ImGui::End();
}

void PerformanceMonitor::InitMonitor()
{
	// This code block is used to find the CPU usage
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	numProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&lastCPU, &ftime, sizeof(FILETIME));

	self = GetCurrentProcess();
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

void PerformanceMonitor::DebugFPS()
{
	std::cout << "FPS: " << 1.0f / Timer::GetDeltaTime()  << std::endl;
}

float PerformanceMonitor::GetFPS()
{
	return 1.0f / Timer::GetDeltaTime();
}

int PerformanceMonitor::GetMemoryUsage()
{
	PROCESS_MEMORY_COUNTERS_EX pmc;
	bool GetMem = GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	const int physicalMemUsed_Bytes = pmc.WorkingSetSize;

	const int physicalMemUsed_MB = physicalMemUsed_Bytes / 1000000;

	return physicalMemUsed_MB;
}

double PerformanceMonitor::GetCPUUsage()
{
	 FILETIME ftime, fsys, fuser;
	 ULARGE_INTEGER now, sys, user;
	 double percent;

	 GetSystemTimeAsFileTime(&ftime);
	 memcpy(&now, &ftime, sizeof(FILETIME));

	 GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	 memcpy(&sys, &fsys, sizeof(FILETIME));
	 memcpy(&user, &fuser, sizeof(FILETIME));
	 percent = static_cast<double>((sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart));
	 percent /= static_cast<double>(now.QuadPart - lastCPU.QuadPart);
	 percent /= numProcessors;
	 lastCPU = now;
	 lastUserCPU = user;
	 lastSysCPU = sys;

	 return percent * 100;
}

#pragma endregion 

#pragma region Viewport

Viewport::Viewport() : viewport_Min(0.0f), viewport_Max(0.0f), viewportSize(0.0f), 
					   modeName("[Result]"), aspectSize("[Free Aspect]"), renderMode(RenderMode::Result), 
					   isMouseHovered(false), isActive(true), MouseX(0), MouseY(0), activeRatio(AspectRatio::FREE_ASPECT)
{
	modeMap.push_back("Result");
	modeMap.push_back("Albedo");
	modeMap.push_back("Position");
	modeMap.push_back("Normals");
	modeMap.push_back("Depth");
	modeMap.push_back("Stencil");

	aspectRatios.push_back("Free Aspect");
	aspectRatios.push_back("3:2");
	aspectRatios.push_back("4:3");
	aspectRatios.push_back("5:3");
	aspectRatios.push_back("5:4");
	aspectRatios.push_back("16:9");
	aspectRatios.push_back("16:10");
}

Viewport::~Viewport() {}

void Viewport::Render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport",&isActive, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();


#pragma region Render Mode/Aspect Ratio

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu(std::string("Mode " + modeName).c_str()))
		{
			if (ImGui::BeginListBox("##RenderModeList"))
			{
				int index = 0;
				for (const auto* Mode : modeMap)
				{
					const RenderMode loopMode = static_cast<RenderMode>(index);

					const bool is_selected = (renderMode == loopMode);
					if (ImGui::Selectable(Mode, is_selected))
					{
						ImGui::CloseCurrentPopup();
						renderMode = loopMode;
						modeName = "[" + std::string(Mode) + "]";
					}
					index++;
				}
				ImGui::EndListBox();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(std::string("Aspect Ratio " + aspectSize).c_str()))
		{
			if (ImGui::BeginListBox("##RenderModeList"))
			{
				int index = 0;
				for (const auto* currentRatio : aspectRatios)
				{
					const AspectRatio ratio = static_cast<AspectRatio>(index);

					const bool is_selected = (activeRatio == ratio);
					if (ImGui::Selectable(currentRatio, is_selected))
					{
						ImGui::CloseCurrentPopup();
						activeRatio = ratio;
						aspectSize = "[" + std::string(currentRatio) + "]";
					}
					index++;
				}
				ImGui::EndListBox();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
#pragma endregion


	if (viewportSize != *reinterpret_cast<MATH::Vec2*>(&viewportPanelSize))
	{
		viewportSize = { viewportPanelSize.x,viewportPanelSize.y };
		Camera* cam = Camera::getInstance();
		cam->setAspectRatio(viewportSize.x / viewportSize.y);
		cam->UpdatePerspectiveMatrix();
		
		Renderer::GetInstance()->Resize(static_cast<int>(viewportPanelSize.x), static_cast<int>(viewportPanelSize.y));
	}

	const GLuint ID = Renderer::GetInstance()->GetModeTextureID();

	ImGui::Image(reinterpret_cast<void*>(ID), ImVec2{ viewportSize.x,viewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	
	UpdateViewportPosition();
	
	ImVec2 mousePos = ImGui::GetMousePos();
	mousePos.x -= viewport_Min.x;
	mousePos.y -= viewport_Min.y;
	mousePos.y = viewportSize.y - mousePos.y;

	 MouseX = (int)mousePos.x;
	 MouseY = (int)mousePos.y;

	if (MouseX >= 0 && MouseY >= 0 && MouseX < (int)viewportSize.x && MouseY < (int)viewportSize.y)
	{
		isMouseHovered = true;
	}
	else
	{
		isMouseHovered = false;
	}

	if (ImGui::BeginDragDropTarget())
	{		
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content_Browser_Object"))
		{
			const std::filesystem::path objPath = static_cast<const char*>(payload->Data);

			SaveFile file = SaveManager::GetSaveFile(objPath.stem().string());

			file.FindAttribute("Identifiers", "UniqueIdentifier") = UUniqueID();

			LoadUtility::GetInstance()->LoadObject(file);
		}
		ImGui::EndDragDropTarget();
	}

	std::shared_ptr<GameObject> SelectedObject = Globals::Editor::GetSelectedObject();

	if (SelectedObject && Globals::Editor::GizmoType != -1)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();


		ImGuizmo::SetRect(viewport_Position.x, viewport_Position.y, viewportSize.x, viewportSize.y + 50);

		MATH::Matrix4 cameraProjection = Camera::getInstance()->getProjectionMatrix();
		MATH::Matrix4 cameraView = Camera::getInstance()->getViewMatrix();

		MATH::Matrix4 ObjectTransform = SelectedObject->transform.GetModelMatrix();

		ImGuizmo::Manipulate(cameraView, cameraProjection, (ImGuizmo::OPERATION)Globals::Editor::GizmoType, ImGuizmo::LOCAL, ObjectTransform);

		if (ImGuizmo::IsUsing())
		{
			MATH::Vec3 position, scale;
			MATH::Quaternion rotQuaternion;
			MATH::MMath::DecomposeTransform(ObjectTransform, position, rotQuaternion, scale);

			switch ((ImGuizmo::OPERATION)Globals::Editor::GizmoType)
			{
			case ImGuizmo::OPERATION::TRANSLATE:
				SelectedObject->transform.SetPos(position);
				break;
			case ImGuizmo::OPERATION::SCALE:
				SelectedObject->transform.SetScale(scale);
				break;
			case ImGuizmo::OPERATION::ROTATE:
				SelectedObject->transform.GetRotationQuatRef() += rotQuaternion;
				SelectedObject->transform.SetRot(SelectedObject->transform.GetRotationQuat().Normalized());
				break;
			}
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void Viewport::UpdateViewportPosition()
{
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();

	ImVec2 Pos = ImGui::GetWindowPos();
	viewport_Position = *reinterpret_cast<MATH::Vec2*>(&Pos);

	if (viewport_Min != *reinterpret_cast<MATH::Vec2*>(&vMin) || viewport_Max != *reinterpret_cast<MATH::Vec2*>(&vMax))
	{
		vMin.x += viewport_Position.x;
		vMin.y += viewport_Position.y;
		vMax.x += viewport_Position.x;
		vMax.y += viewport_Position.y;

		viewport_Min = { vMin.x,vMin.y };
		viewport_Max = { vMax.x,vMax.y };
	}
}

#pragma endregion 

#pragma region ConsoleLog

ConsoleLog::ConsoleLog() : scrollToBottom(true)
{
	EngineLogger::SetCallback(std::bind(&ConsoleLog::AddLog, this, std::placeholders::_1));

	std::ifstream in(EngineLogger::outputName.c_str());

	std::string content;
	while(std::getline(in, content))
	{
		AddLog(content + "\n");
	}
}

ConsoleLog::~ConsoleLog()
{
	Clear();
}

void ConsoleLog::AddLog(const std::string& message)
{
	const void* bufferEnd = message.data() + message.size();

	int oldSize = text.size();
	text.append(message.c_str(), static_cast<const char*>(bufferEnd));
	
	for(int newSize = text.size(); oldSize < newSize; oldSize++)
	{
		if(text[oldSize] == '\n')
		{
			lineOffsets.push_back(oldSize);
		}
		scrollToBottom = true;
	}
}

void ConsoleLog::Render()
{
	ImGui::Begin("Console");

	if(ImGui::Button("Clear"))
	{
		Clear();
	}
	
	ImGui::SameLine();
	const bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,1));

	if(copy)
	{
		ImGui::LogToClipboard();
	}

	if(filter.IsActive())
	{
		const char* textStart = text.begin();
		const char* line = textStart;
		for(int lineNum = 0; line != NULL; lineNum++)
		{
			const char* lineEnd = (lineNum < lineOffsets.size()) ? textStart + lineOffsets[lineNum] : NULL;
			if(filter.PassFilter(line, lineEnd))
			{
				ImGui::TextUnformatted(line, lineEnd);
			}
			line = lineEnd && lineEnd[1] ? lineEnd + 1 : NULL;
		}
	} else
	{
		ImGui::TextUnformatted(text.begin(), text.begin() + text.size());
	}

	if(scrollToBottom)
	{
		ImGui::SetScrollHereY(1.0f);
	}

	scrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::End();
}

#pragma endregion 

#pragma region DockSpace

DockSpace::DockSpace() : dockspaceFlags(ImGuiDockNodeFlags_None), isQueuedForSave(false),isDockSpaceOpen(true), isDockSpaceFullScreen(true)
{
	uiInterfaces.emplace_back(new ContentBrowser());
	uiInterfaces.emplace_back(new HierarchyPanel());
	uiInterfaces.emplace_back(new PerformancePanel());
	uiInterfaces.emplace_back(new PropertiesPanel());
	uiInterfaces.emplace_back(new ConsoleLog());
	uiInterfaces.emplace_back(new NetworkPanel());
	uiInterfaces.emplace_back(new Toolbar());
}

DockSpace::~DockSpace()
{
	for (auto* panel : uiInterfaces)
	{
		delete panel;
		panel = nullptr;
	}
	uiInterfaces.clear();
}

void DockSpace::Update(const float deltatime)
{
	for (auto* panel : uiInterfaces)
	{
		panel->Update(deltatime);
	}
}

void DockSpace::Render()
{
	GenerateDockSpace();
}

void DockSpace::Reset()
{
	for (auto* panel : uiInterfaces)
	{
		panel->Reset();
	}
}

void DockSpace::ConstructUserInterface()
{
	Globals::Editor::SetSelectedObject(nullptr);
	
	for (auto* panel : uiInterfaces)
	{
		panel->Construct();
	}
}

void DockSpace::GenerateDockSpace()
{
	//static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (isDockSpaceFullScreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

	ImGui::Begin("SuperBongoEngine", &isDockSpaceOpen, window_flags);

	if (isDockSpaceFullScreen)
		ImGui::PopStyleVar(2);

	if (isQueuedForSave)
	{
		CoreEngine::GetInstance()->SaveSceneData();
		isQueuedForSave = false;
	}

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			ImGui::MenuItem("Save", NULL, &isQueuedForSave);

			if (ImGui::BeginMenu("Scene"))
			{
				 std::string oldSceneName = CoreEngine::GetInstance()->GetCurrentScene()->GetSceneName();
				 std::string newSceneName = CoreEngine::GetInstance()->GetCurrentScene()->GetSceneName();
				if (ImGui::InputText("##SceneName", &newSceneName, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					Globals::Engine::SCENE_NAME = newSceneName;
					CoreEngine::GetInstance()->GetCurrentScene()->SetSceneName(newSceneName);
					SaveManager::SetSaveName(oldSceneName, newSceneName);
				}
				if (ImGui::Button("New Scene"))
				{
					std::vector<Scene*>& scenes = CoreEngine::GetInstance()->gameInterface->Scenes;

					scenes.push_back(new DefaultScene());
					CoreEngine::GetInstance()->currentSceneNum = scenes.size() - 1;
					SaveUtility::GetInstance()->CreateSave("Default",FileType::SCENE);
				}
				ImGui::EndMenu();
			}
			ImGui::Separator();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	for (auto* panel : uiInterfaces)
	{
		panel->Render();
	}
	ImGui::End();
}

#pragma endregion

#pragma region ContentBrowser

ContentBrowser::ContentBrowser() : ItemPadding(8.0f), ItemSize(64.0f)
{
	CurrentDirectory = "resources";

	GenDirectoryItems();
}

ContentBrowser::~ContentBrowser()
{

}

void ContentBrowser::Render()
{
	ImGui::Begin("Content Browser");



	GeneratePathNav();
	ImGui::Dummy(ImVec2{ 0.0f,25.0f });
	GenerateContent();


	if (!IsHoveringItem && ImGui::IsWindowHovered())
	{
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("Content Dropdown");
		}
	}
	if (QueueForItemMenu)
	{
		ImGui::OpenPopup("Item Menu");
	}



	if (ImGui::BeginPopup("Content Dropdown"))
		{
			if (ImGui::BeginMenu("Create Object"))
			{
				for (const auto& LoadableObject : SaveManager::LoadableObjects)
				{
					std::string PrefixRemoved = LoadableObject.first;
					PrefixRemoved.erase(0, 6);

					if (ImGui::MenuItem(PrefixRemoved.c_str()))
					{
						SaveUtility::GetInstance()->SaveObject(PrefixRemoved, LoadableObject.second->NewClone());

						SaveUtility::GetInstance()->CompileSave(SaveManager::GetSaveFile(PrefixRemoved), std::filesystem::absolute(CurrentDirectory).string() + "\\");

						LoadUtility::GetInstance()->RemoveAttributePrefixs(PrefixRemoved);

						GenDirectoryItems();
					}
				}

				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}
	
	if (ImGui::BeginPopup("Item Menu"))
	{

		if (ImGui::MenuItem("Delete"))
		{
			std::filesystem::path FilePath = CurrentDirectory;

			FilePath.append(SelectedItemName);

			std::filesystem::remove(FilePath);

			GenDirectoryItems();
		}

		ImGui::EndPopup();
	}

	ImGui::End();
}

void ContentBrowser::GenerateContent()
{
	float cellSize = ItemSize + ItemPadding;

	float panelWidth = ImGui::GetContentRegionAvail().x;

	int colCount = (int)(panelWidth / cellSize);

	if (colCount < 1)
	{
		colCount = 1;
	}

	IsHoveringItem = false;
	QueueForItemMenu = false;

	ImGui::Columns(colCount, 0, false);
	for (size_t i = 0; i <  DirectoryItems.size(); i++)
	{
		GenerateItem(DirectoryItems[i]);

	}

	ImGui::Columns(1);
}

void ContentBrowser::GeneratePathNav()
{
	ImGui::Columns(static_cast<int>(DirectoryNames.size()), "Navigation", false);

	float spacing = 30.0f;

	for (size_t i = 0; i < DirectoryNames.size(); i++ )
	{
		if (ImGui::Button(DirectoryNames[i].c_str(), ImVec2{ ImGui::GetColumnWidth(i) - spacing,0.0f }))
		{
			ChangeDirectory(DirectoryNames[i]);
		}
		
		ImGui::SameLine();

		if (i != DirectoryNames.size() - 1)
		{
			ImGui::Text("->");
			ImGui::SameLine();
		}

		ImGui::NextColumn();
	}

	if (DirectoryNames.size() > 1)
	{
		ImGui::Columns(1);
	}
}

void ContentBrowser::GenerateItem(const std::filesystem::directory_entry& entry)
{
	std::filesystem::path path = entry.path();
	std::string filename = entry.path().filename().string();

	if (entry.is_directory())
	{
		if (ImGui::ImageButton((ImTextureID)TextureManager::GetInstance()->GetTexture("FolderIcon.png").getTextureID(), { ItemSize,ItemSize }))
		{
		
		}

		if (!IsHoveringItem) 
		{ 
			IsHoveringItem = ImGui::IsItemHovered(); 
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			CurrentDirectory /= path.filename();
			GenDirectoryItems();
		}
		
		ImGui::TextWrapped(filename.c_str());
	}
	else
	{
		std::string fileType = path.extension().string();

		GLuint iconTextureID;

		if (fileType == ".sbo")
		{
			iconTextureID = TextureManager::GetInstance()->GetTexture("texture_09.jpg").getTextureID();
			ImGui::ImageButton((ImTextureID)iconTextureID, { ItemSize,ItemSize }, ImVec2{ 0.0f,0.0f }, ImVec2{ 1.0f,1.0f }, 1);
		}
		else if (fileType == ".fbx" || fileType == ".obj")
		{
			iconTextureID = TextureManager::GetInstance()->GetTexture("texture_08.jpg").getTextureID();
			ImGui::ImageButton((ImTextureID)iconTextureID, { ItemSize,ItemSize }, ImVec2{ 0.0f,0.0f }, ImVec2{ 1.0f,1.0f }, 1);
		}
		else if (fileType == ".jpg" | fileType == ".png")
		{
			iconTextureID = TextureManager::GetInstance()->GetTexture(path.filename().string()).getTextureID();
			ImGui::ImageButton((ImTextureID)iconTextureID, { ItemSize,ItemSize }, ImVec2{ 0.0f,0.0f }, ImVec2{ 1.0f,1.0f }, 1);
		}
		else
		{
			ImGui::Button(filename.c_str(), { ItemSize,ItemSize });
		}
		if (!IsHoveringItem) { IsHoveringItem = ImGui::IsItemHovered(); }

		if (ImGui::IsItemHovered())
		{
			if (ImGui::BeginDragDropSource())
			{
				std::string fileDir = path.string();

				if (path.extension().string() == ".sbo")
				{
					ImGui::SetDragDropPayload("Content_Browser_Object", fileDir.c_str(), fileDir.size() * sizeof(const char*), ImGuiCond_Once);
				}
				else if (path.extension().string() == ".fbx" || path.extension().string() == ".obj")
				{
					std::string meshName = path.filename().string();

					ImGui::SetDragDropPayload("Content_Browser_Model", meshName.c_str(), meshName.size() * sizeof(const char*), ImGuiCond_Once);
				}
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsMouseDoubleClicked(0))
			{
				if (path.extension().string() == ".scene")
				{
					for (size_t i = 0; i < CoreEngine::GetInstance()->gameInterface->Scenes.size(); i++)
					{
						Scene* scene = CoreEngine::GetInstance()->gameInterface->Scenes[i];

						std::string stem = path.stem().string();
						if (scene->GetSceneName() == stem)
						{
							LoadUtility::GetInstance()->UnLoadSceneSaves();
							CoreEngine::GetInstance()->currentSceneNum = i;
							break;
						}
					}
				}
			}


			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				QueueForItemMenu = true;
				SelectedItemName = filename;
			}
		}

		ImGui::TextWrapped(filename.c_str());		
	}


	ImGui::NextColumn();
}

void ContentBrowser::GenDirectoryItems()
{
	DirectoryItems.clear();
	DirectoryNames.clear();

	for (const auto& entry : std::filesystem::directory_iterator(CurrentDirectory))
	{
		DirectoryItems.push_back(entry);
	}

	// Loops through the directory and takes all of the folder names for later use.
	std::filesystem::path path = CurrentDirectory;
	std::string pathStr = CurrentDirectory.string() + "\\";

	while (!pathStr.empty())
	{
		std::string  Name = pathStr.substr(0, pathStr.find("\\"));

		pathStr.erase(0, pathStr.find("\\") + 1);

		DirectoryNames.push_back(Name);
	}
}

void ContentBrowser::ChangeDirectory(const std::string& dir)
{
	std::string oldDir = CurrentDirectory.string();
	std::string dirString = CurrentDirectory.string();

	oldDir.erase(0, oldDir.find(dir));
	oldDir.erase(oldDir.find(dir), dir.length());
	dirString.erase(dirString.find(oldDir), oldDir.length());

	CurrentDirectory = dirString;
	GenDirectoryItems();
}
#pragma endregion 

void CustomUI::Toolbar::Render()
{
	ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse );
	
	//float size = ImGui::GetWindowHeight() - 4.0f;
	//ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

	float ButtonSize = std::clamp(ImGui::GetWindowHeight() - 20.0f, 10.0f, 50.0f);
	ImGui::Indent((ImGui::GetWindowContentRegionMax().x * 0.5) - ButtonSize);


	if(!CoreEngine::GetInstance()->GetIsGameRunning())
	{
	
		if(ImGui::ImageButton((ImTextureID)TextureManager::GetInstance()->GetTexture("PlayIcon.png").getTextureID(), { ButtonSize,ButtonSize }))
		{
			CoreEngine::GetInstance()->PlayScene();
			EngineLogger::Info("Play pressed", "CustomUI.cpp", __LINE__);
		}
	} 
	else
	{
		if(ImGui::ImageButton((ImTextureID)TextureManager::GetInstance()->GetTexture("StopIcon.png").getTextureID(), { ButtonSize,ButtonSize }))
		{
			CoreEngine::GetInstance()->StopScene();
			EngineLogger::Info("Stop pressed", "CustomUI.cpp", __LINE__);
		}
	}
	ImGui::End();
}
