#include "CustomUI.h"
#include "windows.h"
#include "psapi.h"
#include "scenes/Scene.h"

#include "core/CoreEngine.h"
#include "core/GameInterface.h"
#include "core/Timer.h"
#include "core/resources/TextureManager.h"

#include "imgui/imgui_internal.h"
#include "imgui/imgui_stdlib.h"

#include "UIStatics.h"

#include "components/3D/MeshRenderer.h"


CustomUI::PropertiesPanel::PropertiesPanel() : isActive(true)
{
	
}

CustomUI::PropertiesPanel::~PropertiesPanel()
{
	
}

void CustomUI::PropertiesPanel::Render() 
{
	

	ImGui::Begin("Properties",&isActive);

	if (UIStatics::GetSelectedObject())
	{
		// Gets the mesh's properties and then displays them with ImGui


		ImGui::InputText("Mesh Name", &UIStatics::GetSelectedObject()->name);

		ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;
		bool opened = ImGui::TreeNodeEx((void*)(uint32_t)UIStatics::GetSelectedObject(), tree_flags, "Transform");

		if (opened)
		{
			// Change the standard transform components 

			DrawVec3("Position", UIStatics::GetSelectedObject()->transform.GetPosition(),80.0f);
			DrawVec3("Rotation", UIStatics::GetSelectedObject()->transform.GetRotation(), 80.0f);
			DrawVec3("Scale", UIStatics::GetSelectedObject()->transform.GetScale(), 80.0f);

			ImGui::TreePop();
		}
		
		

		// Create a new color that is a copy of the meshes color


		if (UIStatics::GetSelectedObject()->HasComponent<MeshRenderer>())
		{
			bool opened = ImGui::TreeNodeEx((void*)(uint32_t)UIStatics::GetSelectedObject()->GetComponent<MeshRenderer>(), tree_flags, "Renderer");

			if (opened)
			{
				ImGui::ColorEdit4("Mesh Color", UIStatics::GetSelectedObject()->GetComponent<MeshRenderer>()->meshColorTint);
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();

}

void CustomUI::PropertiesPanel::DrawVec3(const std::string label, MATH::Vec3& value, const float columnWidth)
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







CustomUI::HierarchyPanel::HierarchyPanel() : isActive(true)
{

}

CustomUI::HierarchyPanel::~HierarchyPanel()
{

	if (gameobjects.size() >= 1)
	{
		for (auto obj : gameobjects)
		{
			obj = nullptr;
		}
		gameobjects.clear();

	}

}

void CustomUI::HierarchyPanel::ConstructHierarchy()
{
	UpdateActiveObjects();

	isActive = true;

}

void CustomUI::HierarchyPanel::Render() 
{

	ImGui::Begin("Object Hierarchy", &isActive, ImGuiWindowFlags_NoTitleBar);


	
	textFilter.Draw("##Obj Filter");

	
	for (int i = 0; i < gameobjects.size(); i++)
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

void CustomUI::HierarchyPanel::Update(const float deltatime)
{

	size_t size = UIStatics::GetSceneGraph()->GetGameObjects().size();
	if (gameobjects.size() < size || gameobjects.size() > size)
	{
		UpdateActiveObjects();
	}
}

void CustomUI::HierarchyPanel::GenerateTree(GameObject* go, int index) 
{

	ImGuiTreeNodeFlags tree_flags = ((UIStatics::GetSelectedObject() == go) ? ImGuiTreeNodeFlags_Selected : 0) |
		((go->GetChildCount() == 0) ?  ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen : 0) 
		| ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (go->GetChildCount() > 0)
	{

		bool nodeOpened = ImGui::TreeNodeEx((void*)(uint32_t)go, tree_flags, go->name.c_str());
		
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

				GameObject* movedObj = gameobjects[objIndex];

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

		if (ImGui::IsItemClicked())
		{
			UIStatics::SetSelectedObject(go);
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

		bool nodeOpened = ImGui::TreeNodeEx((void*)(uint32_t)go, tree_flags, go->name.c_str());

		if (ImGui::IsItemClicked())
		{
			UIStatics::SetSelectedObject(go);
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

				GameObject* movedObj = gameobjects[objIndex];
				

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

		
		
		

	}

}

void CustomUI::HierarchyPanel::UpdateActiveObjects()
{
	for (auto obj : UIStatics::GetSceneGraph()->GetGameObjects())
	{
		std::vector<GameObject*>::iterator iter;
		
		iter = std::find(gameobjects.begin(), gameobjects.end(), obj);
		if (iter == gameobjects.end())
		{
			gameobjects.push_back(obj);
		}
	}
}

int CustomUI::HierarchyPanel::GetObjIndex(std::string objName) const
{
	for (int i = 0; i < gameobjects.size(); i++)
	{
		if (gameobjects[i]->name == objName)
		{
			return i;
		}
	}

	

	return -1;
}



int CustomUI::PerformanceMonitor::FPSLimit = 60;
float CustomUI::PerformanceMonitor::RenderLoopTime = 0.0f;
float CustomUI::PerformanceMonitor::UpdateLoopTime = 0.0f;
bool CustomUI::PerformanceMonitor::LimitFPS = false;

 
// Varibables that are used to collect CPU data
ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
int numProcessors;
HANDLE self;

CustomUI::PerformancePanel::PerformancePanel() : latestFPS(0)
{

}

CustomUI::PerformancePanel::~PerformancePanel()
{

}

void CustomUI::PerformancePanel::Update(const float deltatime)
{
	fpsUpdateSpeed -= deltatime;

	if (fpsUpdateSpeed <= 0.0f)
	{
		if (fpsValues.size() == 100)
			fpsValues.erase(fpsValues.begin());

		fpsValues.push_back(PerformanceMonitor::GetFPS());
		fpsUpdateSpeed = initSpeed;

		latestFPS = (int)PerformanceMonitor::GetFPS();
	}
}

void CustomUI::PerformancePanel::Render()
{
	ImGui::Begin("Performance Monitor");

	ImGui::PlotLines("FPS", fpsValues.data(), fpsValues.size()); 	ImGui::SameLine();
	ImGui::Text("%i", latestFPS);


	ImGui::Checkbox("Limit FPS", &PerformanceMonitor::LimitFPS); ImGui::SameLine();
	ImGui::InputInt("", &PerformanceMonitor::FPSLimit);

	ImGui::Text("Render Loop Time %f ms", PerformanceMonitor::RenderLoopTime);
	ImGui::Text("Update Loop Time %f ms", PerformanceMonitor::UpdateLoopTime);
	ImGui::Text("Physical Memory Usage %i MB", PerformanceMonitor::GetMemoryUsage());
	ImGui::Text("CPU Usage %f %%", PerformanceMonitor::GetCPUUsage());


	ImGui::End();
}

void CustomUI::PerformanceMonitor::InitMonitor()
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

void CustomUI::PerformanceMonitor::DebugFPS()
{
	std::cout << "FPS: " << 1/Timer::GetDeltaTime()  << std::endl;
}

float CustomUI::PerformanceMonitor::GetFPS()
{
	return 1 / Timer::GetDeltaTime();
}

int CustomUI::PerformanceMonitor::GetMemoryUsage()
{

	PROCESS_MEMORY_COUNTERS_EX pmc;
	bool GetMem = GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));


	int physicalMemUsed_Bytes = pmc.WorkingSetSize;

	int physicalMemUsed_MB = physicalMemUsed_Bytes / 1000000;

	return physicalMemUsed_MB;
}

double CustomUI::PerformanceMonitor::GetCPUUsage()
{
	 FILETIME ftime, fsys, fuser;
	 ULARGE_INTEGER now, sys, user;
	 double percent;

	 GetSystemTimeAsFileTime(&ftime);
	 memcpy(&now, &ftime, sizeof(FILETIME));

	 GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	 memcpy(&sys, &fsys, sizeof(FILETIME));
	 memcpy(&user, &fuser, sizeof(FILETIME));
	 percent = (sys.QuadPart - lastSysCPU.QuadPart) +
		 (user.QuadPart - lastUserCPU.QuadPart);
	 percent /= (now.QuadPart - lastCPU.QuadPart);
	 percent /= numProcessors;
	 lastCPU = now;
	 lastUserCPU = user;
	 lastSysCPU = sys;

	 return percent * 100;
}




CustomUI::Viewport::Viewport() : viewportSize(0.0f), viewport_Min(0.0f), viewport_Max(0.0f), isMouseHovered(false)
{


}

CustomUI::Viewport::~Viewport()
{
}

void CustomUI::Viewport::Render()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

	if (viewportSize != *(MATH::Vec2*)&viewportPanelSize)
	{
		viewportSize = { viewportPanelSize.x,viewportPanelSize.y };
		
		Renderer::GetInstance()->Resize((int)viewportPanelSize.x, (int)viewportPanelSize.y);

	}

	GLuint ID = Renderer::GetInstance()->GetgBufferTextureID();

	ImGui::Image((ImTextureID)ID, ImVec2{ viewportSize.x,viewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();

	if (viewport_Min != *(MATH::Vec2*)&vMin || viewport_Max != *(MATH::Vec2*)&vMax)
	{
		

		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		viewport_Min = { vMin.x,vMin.y };
		viewport_Max = { vMax.x,vMax.y };

	}
	
	isMouseHovered = ImGui::IsMouseHoveringRect(vMin, vMax);


	if (ImGui::BeginDragDropTarget())
	{		
		const char* path;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Content_Browser_Path"))
		{
			path = (const char*)payload->Data;

			std::filesystem::path objPath = path;

			SaveFile file = SaveManager::GetSaveFile(objPath.stem().string());

			UIStatics::GetSceneGraph()->LoadObject(file);

		}
		ImGui::EndDragDropTarget();
	}


	ImGui::End();
	ImGui::PopStyleVar();

}






CustomUI::DockSpace::DockSpace() : isDockSpaceOpen(true), isDockSpaceFullScreen(true),isQueuedForSave(false), dockspaceFlags(ImGuiDockNodeFlags_None)
{

}

CustomUI::DockSpace::~DockSpace()
{
	
}

void CustomUI::DockSpace::Update(const float deltatime)
{
	performancePanel.Update(deltatime);

	hierarchy.Update(deltatime);
}

void CustomUI::DockSpace::Render()
{
	GenerateDockSpace();
}

void CustomUI::DockSpace::ConstructUserInterface()
{

	UIStatics::ConstructUIStatics();
	hierarchy.ConstructHierarchy();

}

void CustomUI::DockSpace::GenerateDockSpace()
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

	ImGui::Begin("DockSpace Demo", &isDockSpaceOpen, window_flags);


	if (isDockSpaceFullScreen)
		ImGui::PopStyleVar(2);

	if (isQueuedForSave)
	{
		CoreEngine::GetInstance()->GetCurrentScene()->SaveMapData();
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
			ImGui::MenuItem("Fullscreen", NULL, &isDockSpaceFullScreen);
			ImGui::MenuItem("Save", NULL, &isQueuedForSave);
			ImGui::Separator();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}


	hierarchy.Render();

	performancePanel.Render();

	propertiesPanel.Render();

	contentBrowser.Render();

	ImGui::End();
}



CustomUI::ContentBrowser::ContentBrowser() : ItemPadding(8.0f), ItemSize(64.0f)
{
	CurrentDirectory = "resources";

	GenDirectoryItems();
}

CustomUI::ContentBrowser::~ContentBrowser()
{

}

void CustomUI::ContentBrowser::Render()
{

	ImGui::Begin("Content Browser");


	GeneratePathNav();
	ImGui::Dummy(ImVec2{ 0.0f,25.0f });
	GenerateContent();

	ImGui::End();
}

void CustomUI::ContentBrowser::GenerateContent()
{
	float cellSize = ItemSize + ItemPadding;


	float panelWidth = ImGui::GetContentRegionAvail().x;

	int colCount = (int)(panelWidth / cellSize);


	if (colCount < 1)
	{
		colCount = 1;
	}

	ImGui::Columns(colCount, 0, false);
	for (int i = 0; i <  DirectoryItems.size(); i++)
	{
		GenerateItem(DirectoryItems[i]);
	}

	ImGui::Columns(1);

}

void CustomUI::ContentBrowser::GeneratePathNav()
{



	ImGui::Columns((int)DirectoryNames.size(), "Navigation", false);


	


	float spacing = 30.0f;



	for (int i = 0; i < DirectoryNames.size(); i++ )
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

void CustomUI::ContentBrowser::GenerateItem(std::filesystem::directory_entry entry)
{
	std::filesystem::path path = entry.path();
	std::string filename = entry.path().filename().string();


	if (entry.is_directory())
	{

		ImGui::ImageButton((ImTextureID)TextureManager::GetTexture("FolderIcon.png").getTextureID(), { ItemSize,ItemSize });


		

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


		ImGui::Button(filename.c_str(), ImVec2{ ItemSize,ItemSize });

		if (ImGui::BeginDragDropSource())
		{

			std::string fileDir = path.string();
			ImGui::SetDragDropPayload("Content_Browser_Path", fileDir.c_str(), fileDir.size() * sizeof(const char*),ImGuiCond_Once);




			ImGui::EndDragDropSource();
		}


		ImGui::TextWrapped(filename.c_str());


		
	}

	

	ImGui::NextColumn();
}

void CustomUI::ContentBrowser::GenDirectoryItems()
{

	DirectoryItems.clear();
	DirectoryNames.clear();


	for (auto& entry : std::filesystem::directory_iterator(CurrentDirectory))
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

void CustomUI::ContentBrowser::ChangeDirectory(std::string dir)
{
	std::string oldDir = CurrentDirectory.string();
	std::string dirString = CurrentDirectory.string();

	oldDir.erase(0, oldDir.find(dir));
	oldDir.erase(oldDir.find(dir), dir.length());
	dirString.erase(dirString.find(oldDir), oldDir.length());


	CurrentDirectory = dirString;
	GenDirectoryItems();
}




