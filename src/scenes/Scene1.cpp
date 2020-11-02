
#include "Scene1.h"
#include "custom/Player.h"
#include "core/Debug.h"
#include "custom/MouseRay.h"

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
	player = new Player("Player", MATH::Vec3(0.0f, 5.0f, 90.0f));

	grass = new Grass("Grass", MATH::Vec3(0.0f, 4.0f, 0.0f), 500);
	plane = new Plane("Plane", MATH::Vec3(0.0f, 0.0f, 0.0f));
	//fog = new TestModel("Fog", MATH::Vec3(0.0f, 10.0f, 0.0f));
	light = new LightObject("Light", MATH::Vec3(0.0f, 20.0f, 0.0f));
	mouseRay = new MouseRay();
	objectList->AddGameObject(player,1);
	objectList->AddGameObject(grass, 2);
	objectList->AddGameObject(plane, 3);
	//objectList->AddGameObject(fog, 4);
	objectList->AddGameObject(light, 5);

	//This init function separates any gameobjects that have rigidbodies for their physics calculations
	objectList->Init();

	SaveMapData();
	//LoadMapData();
	return false;
}



void Scene1::OnDestroy()
{
	delete mouseRay;
	delete objectList;
	objectList = nullptr;
}

void Scene1::Update(const float deltaTime)
{
	//std::cout << 1.0f / deltaTime << std::endl;
	Camera::getInstance()->Update(deltaTime);
	objectList->CheckCollisions();
	objectList->Update(deltaTime);

	//if (CheckIntersection(mouseRay, Camera::getInstance()->getPosition()))
	//{
	//	std::cout << "Hit Something" << std::endl;
	//}

	//mouseRay->Update(deltaTime);
}



void Scene1::Render() const
{
	objectList->Render();
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	mouseRay->HandleEvents(event);
	objectList->HandleEvents(event);
}

void Scene1::Reset()
{
	OnDestroy();
	OnCreate();
}

void Scene1::SaveMapData()
{
	Scene::SaveMapData();
}

void Scene1::LoadMapData()
{
	Scene::LoadMapData();	
}

bool Scene1::CheckIntersection(MouseRay* ray, const Vec3& origin)
{
	Vec3 boxmin = fog->getComponent<MeshRenderer>().GetMinVector();
	Vec3 boxmax = fog->getComponent<MeshRenderer>().GetMaxVector();

	//Check x
	float tmin = (boxmin.x - origin.x) / ray->GetCurrentRay().x;
	float tmax = (boxmax.x - origin.x) / ray->GetCurrentRay().x;

	if (tmin > tmax) std::swap(tmin, tmax);

	// Check y
	float tymin = (boxmin.y - origin.y) / ray->GetCurrentRay().y;
	float tymax = (boxmax.y - origin.y) / ray->GetCurrentRay().y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (boxmin.z - origin.z) / ray->GetCurrentRay().z;
	float tzmax = (boxmin.z - origin.z) / ray->GetCurrentRay().z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}




	// Other Method
	/*
	Matrix4 MultiMatrix;
	for (int i = 0; i < objectList->GetNumObjects(); i++)
	{
		MultiMatrix = Camera::getInstance()->getProjectionMatrix() * Camera::getInstance()->getViewMatrix() 
			* objectList->GetGameObjects()[i]->GetModelMatrix();
	}

	Matrix4 inverseMulti = MMath::inverse(MultiMatrix);

	Vec4 Position = Vec4(x_, y_, -1.0, 1.0);

	Position = inverseMulti * Position;

	Position.w = 1.0 / Position.w;

	Position.x *= Position.w;
	Position.y *= Position.w;
	Position.z *= Position.w;



	if (VMath::distance(Position,fog->transform.pos) < 20.0f)
	{
		std::cout << "Mouse Clicked object" << std::endl;
	}
	Position.print();*/

	

