
#include "Scene1.h"
#include "custom/Player.h"
#include "custom/MouseRay.h"
#include "core/Logger.h"


bool Scene1::OnCreate()
{
	EngineLogger::Info("Scene 1 Created", "Scene1.cpp", __LINE__);
	objectList = new Manager();

	//Setup the player
	player = new Player("Player", MATH::Vec3(0.0f, 20.0f, 70.0f));

	grass = new Grass("Grass", MATH::Vec3(0.0f, 1.0f, 0.0f), 700);
	plane = new Plane("Plane", MATH::Vec3(0.0f, 0.0f, 0.0f));
	//fog = new TestModel("Fog", MATH::Vec3(0.0f, 10.0f, 0.0f));
	light = new LightObject("Light", MATH::Vec3(0.0f, 20.0f, 0.0f));
	bird = new Bird("bird", MATH::Vec3(20.0f, 0.0f, 20.0f));
	mouseRay = MouseRay();
	objectList->AddGameObject(player,1);
	objectList->AddGameObject(grass, 2);
	objectList->AddGameObject(plane, 3);
	//objectList->AddGameObject(fog, 4);
	objectList->AddGameObject(light, 5);
	objectList->AddGameObject(bird, 7);

	objectList->Init();

	PerformanceMonitor::InitMonitor();
	
	//Scene::SaveMapData();
	Scene::LoadMapData();
	return true;
}

void Scene1::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
	Camera::getInstance()->Update(deltaTime);
	objectList->CheckCollisions();
	objectList->Update(deltaTime);

}



void Scene1::Render() const
{
	
	Scene::Render();
	objectList->Render();
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	Scene::HandleEvents(event);
	objectList->HandleEvents(event);
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





