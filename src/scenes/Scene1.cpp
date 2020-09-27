#include "Scene1.h"
#include "custom/Player.h"
#include "tiles/Tilemap.h"
#include "core/Debug.h"

Scene1::Scene1()
{}

Scene1::~Scene1()
{
	
}

bool Scene1::OnCreate()
{
	
	std::cout << "scene1 loaded" << std::endl;
	objectList = new Manager();

	//Setup the player
	player = new Player("Player", MATH::Vec3(0.0f, 10.0f, 30.0f));
	objectList->AddGameObject(player);

	ball = new TestModel("Ball", MATH::Vec3(0.0f, 80.0f, 15.0f));
	plane = new Plane("Plane", MATH::Vec3(0.0f, 0.0f, 0.0f));
	objectList->AddGameObject(ball);
	objectList->AddGameObject(plane);

	light = new LightObject("Light1", MATH::Vec3(0.0f, 30.0f, 0.0f));
	objectList->AddGameObject(light);
	//This init function separates any gameobjects that have rigidbodies for their physics calculations
	objectList->Init();

	return false;
}



void Scene1::OnDestroy()
{
	delete objectList;
	objectList = nullptr;
}

void Scene1::Update(const float deltaTime)
{
	//std::cout << 1.0f / deltaTime << std::endl;
	Camera::getInstance()->Update(deltaTime);
	objectList->CheckCollisions();
	objectList->Update(deltaTime);

	light->Render();
}



void Scene1::Render() const
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	objectList->Render();
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	objectList->HandleEvents(event);
}

void Scene1::Reset()
{
	OnDestroy();
	OnCreate();
}
