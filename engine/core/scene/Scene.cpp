#include "Scene.h"

#include <imgui/imgui_internal.h>
#include <ImGuizmo/ImGuizmo.h>

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

#include "Rendering/Renderer.h"

using namespace MATH;

Scene::Scene() : Scene_Name("Scene_"), objectList(std::make_shared<SceneGraph>())
{
	
}

Scene::~Scene()
{

}

bool Scene::OnCreate()
{
	EngineLogger::Info("Scene: " + Scene_Name + " Created", "Scene.cpp", __LINE__);
	
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
	objectList->HandleEvents(event);


	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym) {
		case SDLK_q:
			UIStatics::GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case SDLK_r:
			UIStatics::GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case SDLK_e:
			UIStatics::GizmoType = ImGuizmo::OPERATION::SCALE;

			break;
			// etc
		}
	}

	


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


		int XPos = Renderer::GetInstance()->GetViewport().GetMousePosX();

		int YPos = Renderer::GetInstance()->GetViewport().GetMousePosY();


		uint32_t PixelData = Renderer::GetInstance()->gBuffer.ReadPixel(5, XPos, YPos);
		

		std::shared_ptr<GameObject> SelectedObj = objectList->FindGameObject(PixelData);

		if (SelectedObj != nullptr)
		{
			SelectedObj->isObjectSelected = true;
			UIStatics::SetSelectedObject(SelectedObj);
		}
		
	}

}

void Scene::SaveMapData() const
{
	if (!SaveManager::TransferToSaveQueue(Scene_Name))
	{
		SaveUtility::GetInstance()->CreateSave(Scene_Name, FileType::SCENE);
	}

	SaveManager::GetSaveFile(Scene_Name).ClearElements();
	ElementInfo info = ElementInfo("Root");

	SaveUtility::GetInstance()->AddElement(Scene_Name, "SceneSettings", info);
	info = ElementInfo("SceneSettings");
	info.Attributes.emplace("S_:", std::string(typeid(*this).name()));
	SaveUtility::GetInstance()->AddElement(Scene_Name, "BaseClass:", info);

	info = ElementInfo("Root");
	SaveUtility::GetInstance()->AddElement(Scene_Name, "Objects", info);
	
	info = ElementInfo("Objects");
	
	for (const auto& obj : objectList->GetGameObjects())
	{
		SaveUtility::GetInstance()->AddElement(Scene_Name, obj->GetName(), info);
		SaveUtility::GetInstance()->SaveObject(obj->GetName(), obj);
	}

	SaveUtility::GetInstance()->CompileSaves();
}

void Scene::LoadMapData()
{
	for (auto elm : SaveManager::GetSaveFile(Scene_Name).GetElements())
	{
		if (!objectList->FindGameObject(elm.first))
		{
			LoadUtility::GetInstance()->LoadObject(SaveManager::GetSaveFile(elm.first));
		}
	}
}