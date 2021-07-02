
#include "Scene1.h"
#include "gameObjects/Player.h"
#include "core/MouseRay.h"
#include "core/Logger.h"
#include "gameObjects/LightObject.h"
#include "gameObjects/Bird.h"
#include "Primitives/PlaneObject.h"
#include "gameObjects/Grass.h"
#include "core/resources/SaveManager.h"
const char* GetType(GameObject& go)
{
	return typeid(go).name();
}

bool Scene1::OnCreate()
{
	EngineLogger::Info("Scene 1 Created", "Scene1.cpp", __LINE__);

	//Setup the player
	Player* player = new Player("Player", MATH::Vec3(0.0f, 20.0f, 70.0f));

	Grass* grass = new Grass("Grass", MATH::Vec3(0.0f, 1.0f, 0.0f), 700);
	PlaneObject* plane = new PlaneObject("FirstPlane", MATH::Vec3(0.0f, 0.0f, 10.0f));
	//PlaneObject* plane2 = new PlaneObject("SecondPlane", MATH::Vec3(0.0f, 0.0f, -10.0f));
	//TestModel* fog = new TestModel("Fog", MATH::Vec3(0.0f, 1000.0f, 0.0f));
	LightObject* light = new LightObject("Light", MATH::Vec3(0.0f, 20.0f, 0.0f));
	//Bird* bird = new Bird("bird", MATH::Vec3(20.0f, 0.0f, 20.0f));
	mouseRay = MouseRay();
	objectList->AddGameObject(player);
	objectList->AddGameObject(grass);
	objectList->AddGameObject(plane);
	//objectList->AddGameObject(plane2);
	//objectList->AddGameObject(fog);
	objectList->AddGameObject(light);
	//objectList->AddGameObject(bird);

	objectList->Init();
	
	PerformanceMonitor::InitMonitor();
	
	SaveUtility::GetInstance()->CreateSave("TestSave",FileType::SCENE);

	ElementInfo testInfo;
	testInfo.parentName = "Root";

	SaveUtility::GetInstance()->CompileSaves();

	return true;

}

void Scene1::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
	Camera::getInstance()->Update(deltaTime);
}

void Scene1::Render() const
{
	Scene::Render();
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	Scene::HandleEvents(event);
}

void Scene1::Reset()
{
	OnDestroy();
	OnCreate();
}

void Scene1::SaveMapData() const 
{
	Scene::SaveMapData();
}

void Scene1::LoadMapData()
{	
	Scene::LoadMapData();
}





