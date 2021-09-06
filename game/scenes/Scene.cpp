#include "Scene.h"
#include "gameObjects/TestModel.h"
#include "Primitives/PlaneObject.h"
#include <math.h>
#include "core/MouseRay.h"
#include "core/Logger.h"
#include "core/3D/Physics3D.h"
#include "core/Globals.h"

#include "imgui/imgui_internal.h"
using namespace MATH;



void Scene::OnMouseMove(MATH::Vec2 mouse)
{
	
}

void Scene::OnMousePressed(MATH::Vec2 mouse, int buttonType)
{
	if (buttonType == SDL_BUTTON_LEFT)
	{
		mouseRay.CalaculateMouseRay();

		//GameObject* hitResult = objectList->osp.GetCollision(mouseRay);
		GameObject* hitResult = nullptr;
		float shortestDistance = FLT_MAX;
		for (auto* obj : objectList->GetGameObjects())
		{
			if (obj->HasComponent<MeshRenderer>())
			{
				MeshRenderer* mr = obj->GetComponent<MeshRenderer>();
				if (Physics3D::RayOBBDetect(mouseRay, mr->OBB))//(CheckIntersection(mouseRay, mouseRay.GetCurrentRay().Origin, obj))
				{
					if(mouseRay.intersectionDist < shortestDistance)
					{
						hitResult = obj;
						shortestDistance = mouseRay.intersectionDist;
					}
				}
			}
		}

		if(hitResult)
		{
			EngineLogger::Info("Mouse hit " + std::string(hitResult->name), "Scene.cpp", __LINE__);
			if (!hitResult->isMenuActive)
			{
				CheckExistingPanel(hitResult);
				propertiesPanels.push_back(new CustomUI::PropertiesPanel(hitResult));
			}
			hitResult->isMenuActive = true;
		}
	}
}

void Scene::CreateObjWithID(const Vec3& pos_, const Vec3& rot_, const Vec3& scale_, const char* objName_, std::string objType) const
{

	for (auto obj : objectList->GetInstantiableObjects())
	{
		if (obj.first == objType)
		{
			GameObject* clone = obj.second->GetClone();
			clone->SetName(objName_);
			clone->SetPos(pos_);
			clone->SetRotation(rot_);
			clone->SetScale(scale_);
			objectList->AddGameObject(clone);
			return;
		}
	}

	EngineLogger::Warning(objType +" Could not be instantiated","Scene.cpp", __LINE__);
}

 void Scene::CheckExistingPanel(GameObject* obj)
 {
	 for (size_t i = 0; i < propertiesPanels.size(); i++)
	 {
		 if (propertiesPanels[i]->selectedObj->name == obj->name)
		 {
			 propertiesPanels.erase(propertiesPanels.begin() + i);
		 }
	 }
 }

bool Scene::CheckIntersection(const MouseRay& ray, const Vec3& origin, GameObject* obj) const
{
	Vec3 bounds[2];
	bounds[0] = obj->GetComponent<MeshRenderer>()->GetMinVector();


	bounds[1] = obj->GetComponent<MeshRenderer>()->GetMaxVector();

			
	const float tx1 = ((bounds[0].x - origin.x) + obj->transform.pos.x) * ray.invDir.x;
	const float tx2 = ((bounds[1].x - origin.x) + obj->transform.pos.x) * ray.invDir.x;
																		
	float tmin = std::min(tx1, tx2);									
	float tmax = std::max(tx1, tx2);									
																		
	const float ty1 = ((bounds[0].y - origin.y) + obj->transform.pos.y) * ray.invDir.y;
	const float ty2 = ((bounds[1].y - origin.y) + obj->transform.pos.y) * ray.invDir.y;
																		 
	tmin = std::max(tmin, std::min(ty1, ty2));							 
	tmax = std::min(tmax, std::max(ty1, ty2));							 
																		 
	const float tz1 = ((bounds[0].z - origin.z) + obj->transform.pos.z) * ray.invDir.z;
	const float tz2 = ((bounds[1].z - origin.z) + obj->transform.pos.z) * ray.invDir.z;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));


	return tmax >= tmin;
}


Scene::Scene() : name_(new char()), objectList(std::make_unique<SceneGraph>())
{
	objectList = std::make_unique<SceneGraph>();
}

Scene::~Scene()
{

	if(name_)
	{
		delete name_;
		name_ = nullptr;
	}

	for(auto panel : propertiesPanels)
	{
		if(panel)
		{
			delete panel;
			panel = nullptr;
		}
	}
	propertiesPanels.clear();
}

bool Scene::PostCreate()
{
	std::vector<GameObject*> gameObjects;
	
	for (auto object : objectList->GetGameObjects())
	{
		if(object->GetParent() == nullptr)
		{
			gameObjects.push_back(object);
		}
	}
	
	hierarchyPanel.ConstructHierarchy(gameObjects);

	return true;
}

void Scene::Update(const float deltaTime)
{
	objectList->Update(deltaTime);
	objectList->CheckCollisions();
	performancePanel.Update(deltaTime);
}

void Scene::Render() const
{
	ImGui::NewFrame();
	// Let's the use add game objects

	for (size_t i = 0; i < propertiesPanels.size(); i++)
	{
		propertiesPanels[i]->Render();
	}

	 performancePanel.Render();
	 hierarchyPanel.Render();

	 // Displays panel that allows user to add gameobjects at runtime
	 bool enabled = true;
	 ImGui::Begin("Add Game Object", &enabled);
	
	 static std::string selectedObj;

	 ImGui::ListBoxHeader("Test Level");
	 for (auto obj : objectList->GetInstantiableObjects())
	 {
		 if (ImGui::Selectable(obj.first.c_str(), false,ImGuiSelectableFlags_SelectOnClick))
		 {
			 selectedObj = obj.first;
		 }
	 }
	 ImGui::ListBoxFooter();

	// Blank variables that can be changed 
	static Vec3 Pos_ = Vec3(0.0f);
	static Vec3 Rot_ = Vec3(0.0f);
	static Vec3 Scale_ = Vec3(1.0f);


	if (ImGui::InputText("Mesh Name", name_, 20))
	{


	}

	ImGui::DragFloat3("Position", Pos_);
	ImGui::DragFloat3("Rotation", Rot_);
	ImGui::DragFloat3("Scale", Scale_, -1, 1);
	
	if (ImGui::Button("Create Object"))
	{
		CreateObjWithID(Pos_, Rot_, Scale_, name_, selectedObj);
		Pos_ = Vec3(0.0f);
		Rot_ = Vec3(0.0f);
		Scale_ = Vec3(1.0f);
	}
	ImGui::End();

	objectList->Render();
}


void Scene::HandleEvents(const SDL_Event& event)
{
	mouseRay.HandleEvents(event);
	objectList->HandleEvents(event);
}

void Scene::SaveMapData() const
{

	if (SaveManager::HasSave("Scene_1"))
	{
		SaveManager::TransferToSaveQueue("Scene_1");
	}
	else
	{
		SaveUtility::GetInstance()->CreateSave("Scene_1", FileType::SCENE);
	}


	ElementInfo info = ElementInfo("Root");

	SaveUtility::GetInstance()->AddElement("Scene_1", "Objects", info);


	std::string filetype = ".sbo";
	 info = ElementInfo("Objects");
	for (auto obj : objectList->GetGameObjects())
	{
		SaveUtility::GetInstance()->AddElement("Scene_1", obj->name, info);

		SaveUtility::GetInstance()->SaveObject(obj->name, obj);
	}

	SaveUtility::GetInstance()->CompileSaves();



}

void Scene::LoadMapData()
{


	std::string objectDirectory;

	objectDirectory = Globals::ENGINE_PATH;
	objectDirectory.append("\\resources\\SaveData\\Objects\\");
	
	for (auto elm : SaveManager::GetSaveFile("Scene_1").GetElements())
	{
		objectList->LoadObject(SaveManager::GetSaveFile(elm.first));
		
	}
}