#include "Scene.h"

#include <imgui/imgui_internal.h>

#include "core/Globals.h"
#include "core/Logger.h"
#include "core/MouseRay.h"
#include "core/CoreEngine.h"
#include "core/3D/OctSpatialPartition.h"
#include "core/resources/CollisionDetection.h"
#include "core/resources/SaveManager.h"

#include "gameObjects/TestModel.h"
#include "graphics/UIStatics.h"
#include "Utility/LoadUtility.h"

using namespace MATH;

Scene::Scene() : Scene_Name("Scene_" + std::to_string(std::rand())), objectList(std::make_shared<SceneGraph>())
{
	
}

Scene::~Scene()
{

}

bool Scene::OnCreate()
{
	EngineLogger::Info("Scene: " + Scene_Name + " Created", "Scene1.cpp", __LINE__);
	
	objectList->Init();

	return true;
}

bool Scene::PostCreate()
{
	objectList->PostInit();
	return true;
}

void Scene::Update(const float deltaTime)
{
	objectList->Update(deltaTime);
	objectList->CheckCollisions();
	Camera::getInstance()->Update(deltaTime);
}

void Scene::Render() 
{
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

void Scene::OnMousePressed(Vec2 mouse, int buttonType)
{
	if (CoreEngine::GetInstance()->GetCurrentScene() != this)
		return;

	if (buttonType == SDL_BUTTON_LEFT)
	{
		if (!Renderer::GetInstance()->GetViewport().GetIsMouseHovered())
			return;

		mouseRay.CalculateMouseRay();

		GameObject* hitResult = nullptr;
		float shortestDistance = FLT_MAX;

		hitResult = objectList->GetScenePartition()->GetCollision(mouseRay);


		if (hitResult)
		{
			EngineLogger::Info("Mouse hit " + std::string(hitResult->name), "Scene.cpp", __LINE__);
			if (!hitResult->isObjectSelected)
			{
				
			}
			hitResult->isObjectSelected = true;
			UIStatics::SetSelectedObject(hitResult);
		}
	}
}

void Scene::CreateObjWithID(const Vec3& pos_, const Vec3& rot_, const Vec3& scale_, const std::string& objName_, const std::string& objType) const
{

	for (auto obj : objectList->GetInstantiableObjects())
	{
		if (obj.first == objType)
		{
			GameObject* clone = obj.second->NewClone();
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
	
	for (auto* obj : objectList->GetGameObjects())
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