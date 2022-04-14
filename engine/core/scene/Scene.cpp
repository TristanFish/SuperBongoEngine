#include "Scene.h"


#include "core/Logger.h"
#include "core/CoreEngine.h"
#include "core/resources/CollisionDetection.h"


#include "components/GameObject.h"
#include "Rendering/CameraManager.h"
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
	EngineLogger::Info("Scene PostCreate Called", "Scene.h", __LINE__);
	objectList->PostInit();
	return true;
}

void Scene::Update(const float deltaTime)
{
	objectList->Update(deltaTime);
	objectList->CheckCollisions();
	CameraManager::GetInstance()->GetCamera()->Update(deltaTime);
}

void Scene::Render() 
{
	objectList->Render();
}

void Scene::HandleEvents(const SDL_Event& event)
{
	objectList->HandleEvents(event);
}

void Scene::Reset()
{
	objectList->Clear();
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
			Globals::Editor::SetSelectedObject(SelectedObj);
		}
	}
}