#include "Scene.h"

#include <imgui/imgui_internal.h>

#include "core/Globals.h"
#include "core/Logger.h"
#include "core/MouseRay.h"
#include "core/3D/Physics3D.h"
#include "gameObjects/TestModel.h"
using namespace MATH;



Scene::Scene() : Scene_Name("Scene_" + std::to_string(std::rand())), objectList(std::make_shared<SceneGraph>())
{
	
}

Scene::~Scene()
{

}

bool Scene::PostCreate()
{
	

	dockSpace.ConstructUserInterface();

	return true;
}

void Scene::Update(const float deltaTime)
{
	objectList->Update(deltaTime);
	objectList->CheckCollisions();
	dockSpace.Update(deltaTime);
}

void Scene::Render() 
{
	ImGui::NewFrame();
	// Let's the use add game objects

	dockSpace.Render();

	objectList->Render();
}


void Scene::HandleEvents(const SDL_Event& event)
{
	mouseRay.HandleEvents(event);
	objectList->HandleEvents(event);
}



void Scene::OnMouseMove(MATH::Vec2 mouse)
{

}

void Scene::OnMousePressed(MATH::Vec2 mouse, int buttonType)
{
	if (buttonType == SDL_BUTTON_LEFT)
	{
		if (!dockSpace.IsMouseOverViewPort())
			return;

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
					if (mouseRay.intersectionDist < shortestDistance)
					{
						hitResult = obj;
						shortestDistance = mouseRay.intersectionDist;
					}
				}
			}
		}

		if (hitResult)
		{
			EngineLogger::Info("Mouse hit " + std::string(hitResult->name), "Scene.cpp", __LINE__);
			if (!hitResult->isObjectSelected)
			{
				
			}
			hitResult->isObjectSelected = true;
		}
	}
}

void Scene::CreateObjWithID(const Vec3& pos_, const Vec3& rot_, const Vec3& scale_, std::string objName_, std::string objType) const
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

	EngineLogger::Warning(objType + " Could not be instantiated", "Scene.cpp", __LINE__);
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

void Scene::SaveMapData() const
{
	if (!SaveManager::TransferToSaveQueue(Scene_Name))
	{
		SaveUtility::GetInstance()->CreateSave(Scene_Name, FileType::SCENE);
	}
	
	
	ElementInfo info = ElementInfo("Root");


	SaveUtility::GetInstance()->AddElement(Scene_Name, "SceneSettings", info);
	info = ElementInfo("SceneSettings");
	info.Attributes.emplace(":", std::string(typeid(*this).name()));
	SaveUtility::GetInstance()->AddElement(Scene_Name, "BaseClass:", info);




	info = ElementInfo("Root");
	SaveUtility::GetInstance()->AddElement(Scene_Name, "Objects", info);



	
	info = ElementInfo("Objects");
	

	

	
	for (auto obj : objectList->GetGameObjects())
	{
		SaveUtility::GetInstance()->AddElement(Scene_Name, obj->name, info);

		SaveUtility::GetInstance()->SaveObject(obj->name, obj);
	}

	SaveUtility::GetInstance()->CompileSaves();



}

void Scene::LoadMapData()
{
	for (auto elm : SaveManager::GetSaveFile(Scene_Name).GetElements())
	{
		if (!objectList->isObjectActive(elm.first))
		{
			LoadUtility::GetInstance()->LoadObject(SaveManager::GetSaveFile(elm.first));

		}
	}
}