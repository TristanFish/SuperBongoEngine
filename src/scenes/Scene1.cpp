
#include "Scene1.h"
#include "custom/Player.h"
#include "tiles/Tilemap.h"
#include "custom/MouseRay.h"
#include "core/Debug.h"
#include "core/TextureManager.h"


Scene1::Scene1()
{}

Scene1::~Scene1()
{
	
}

bool Scene1::OnCreate()
{
	
	std::cout << "=================\n" << "scene1 load start" << std::endl << "=================\n";
	objectList = new Manager();

	//Setup the player
	player = new Player("Player", MATH::Vec3(0.0f, 20.0f, 70.0f));

	grass = new Grass("Grass", MATH::Vec3(0.0f, 1.0f, 0.0f), 700);
	plane = new Plane("Plane", MATH::Vec3(0.0f, 0.0f, 0.0f));
	//fog = new TestModel("Fog", MATH::Vec3(0.0f, 10.0f, 0.0f));
	light = new LightObject("Light", MATH::Vec3(0.0f, 20.0f, 0.0f));

	mouseRay = new MouseRay();
	objectList->AddGameObject(player,1);
	objectList->AddGameObject(grass, 2);
	objectList->AddGameObject(plane, 3);
	//objectList->AddGameObject(fog, 4);
	objectList->AddGameObject(light, 5);

	objectList->Init();

	PerformanceMonitor::InitMonitor();
	
	//Scene::SaveMapData();
	Scene::LoadMapData();
	return false;
}



void Scene1::OnDestroy()
{
	delete objectList;
	objectList = nullptr;

	delete mouseRay;
	mouseRay = nullptr;

	pElement = nullptr;
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





