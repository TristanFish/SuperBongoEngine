#include "SceneAi.h"

#include "core/Logger.h"
#include "core/Globals.h"
#include "gameObjects/AiGameObject.h"
#include "Primitives/Sphere.h"
#include "Primitives/Box.h"


bool SceneAi::OnCreate()
{
	EngineLogger::Info("Scene Ai Created", "SceneAi.cpp", __LINE__);

	Globals::InitGlobals();
	CustomUI::PerformanceMonitor::InitMonitor();

	

	/*AiGameObject* mainAi = new AiGameObject("AiObj",Vec3(1.0f));
	objectList->AddGameObject(mainAi);

	Sphere* targetSphere = new Sphere("targetSphere",Vec3(15.0f, 2.5f, 7.0f));
	objectList->AddGameObject(targetSphere);
	
	mainAi->aiTarget = targetSphere;*/

	//GJK Code test;
	Box* box1 = new Box("Box1", Vec3(25.0f, 0.0f, 0.0f));
	
	//mesh1 = new Mesh(box1->GetComponent<MeshRenderer>()->GetMeshes().front());
	//for (auto vertex : mesh1->vertices) {

	//	vertex.position = vertex.position + Vec3(25.0f, 0.0f, 0.0f);
	//	//std::cout << vertex.position << std::endl;
	//	int test = 0;
	//}

	objectList->AddGameObject(box1);

	Box* box2 = new Box("Box2", Vec3(-25.0f, 0.0f, 0.0f));
	/*mesh2 = new Mesh(box2->GetComponent<MeshRenderer>()->GetMeshes().front());
	for (auto vertex : mesh1->vertices) {
		vertex.position += Vec3(-25.0f, 0.0f, 0.0f);
		std::cout << vertex.position << std::endl;
	}*/
	objectList->AddGameObject(box2);

	
	{
		mesh1Verts.emplace_back(Vec3(7.5, -17.5, 0));
		mesh1Verts.emplace_back(Vec3(7.5, 17.5, 0));
		mesh1Verts.emplace_back(Vec3(42.5, 17.5, 0));
		mesh1Verts.emplace_back(Vec3(42.5, -17.5, 0));
		mesh1Verts.emplace_back(Vec3(7.5, -17.5, 35));
		mesh1Verts.emplace_back(Vec3(42.5, -17.5, 35));
		mesh1Verts.emplace_back(Vec3(42.5, 17.5, 35));
		mesh1Verts.emplace_back(Vec3(7.5, 17.5, 35));
		mesh1Verts.emplace_back(Vec3(7.5, -17.5, 0));
		mesh1Verts.emplace_back(Vec3(42.5, -17.5, 0));
		mesh1Verts.emplace_back(Vec3(42.5, -17.5, 35));
		mesh1Verts.emplace_back(Vec3(7.5, -17.5, 35));
		mesh1Verts.emplace_back(Vec3(42.5, -17.5, 0));
		mesh1Verts.emplace_back(Vec3(42.5, 17.5, 0));
		mesh1Verts.emplace_back(Vec3(42.5, 17.5, 35));
		mesh1Verts.emplace_back(Vec3(42.5, -17.5, 35));
		mesh1Verts.emplace_back(Vec3(42.5, 17.5, 0));
		mesh1Verts.emplace_back(Vec3(7.5, 17.5, 0));
		mesh1Verts.emplace_back(Vec3(7.5, 17.5, 35));
		mesh1Verts.emplace_back(Vec3(42.5, 17.5, 35));
		mesh1Verts.emplace_back(Vec3(7.5, 17.5, 0));
		mesh1Verts.emplace_back(Vec3(7.5, -17.5, 0));
		mesh1Verts.emplace_back(Vec3(7.5, -17.5, 35));
		mesh1Verts.emplace_back(Vec3(7.5, 17.5, 35));
	}

	{
		mesh2Verts.emplace_back(Vec3(-42.5, -17.5, 0));
		mesh2Verts.emplace_back(Vec3(-42.5, 17.5, 0));
		mesh2Verts.emplace_back(Vec3(-7.5, 17.5, 0));
		mesh2Verts.emplace_back(Vec3(-7.5, -17.5, 0));
		mesh2Verts.emplace_back(Vec3(-42.5, -17.5, 35));
		mesh2Verts.emplace_back(Vec3(-7.5, -17.5, 35));
		mesh2Verts.emplace_back(Vec3(-7.5, 17.5, 35));
		mesh2Verts.emplace_back(Vec3(-42.5, 17.5, 35));
		mesh2Verts.emplace_back(Vec3(-42.5, -17.5, 0));
		mesh2Verts.emplace_back(Vec3(-7.5, -17.5, 0));
		mesh2Verts.emplace_back(Vec3(-7.5, -17.5, 35));
		mesh2Verts.emplace_back(Vec3(-42.5, -17.5, 35));
		mesh2Verts.emplace_back(Vec3(-7.5, -17.5, 0));
		mesh2Verts.emplace_back(Vec3(-7.5, 17.5, 0));
		mesh2Verts.emplace_back(Vec3(-7.5, 17.5, 35));
		mesh2Verts.emplace_back(Vec3(-7.5, -17.5, 35));
		mesh2Verts.emplace_back(Vec3(-7.5, 17.5, 0));
		mesh2Verts.emplace_back(Vec3(-42.5, 17.5, 0));
		mesh2Verts.emplace_back(Vec3(-42.5, 17.5, 35));
		mesh2Verts.emplace_back(Vec3(-7.5, 17.5, 35));
		mesh2Verts.emplace_back(Vec3(-42.5, 17.5, 0));
		mesh2Verts.emplace_back(Vec3(-42.5, -17.5, 0));
		mesh2Verts.emplace_back(Vec3(-42.5, -17.5, 35));
		mesh2Verts.emplace_back(Vec3(-42.5, 17.5, 35));
	}
	

	objectList->Init();
	
	
	return true;
}

void SceneAi::Update(const float deltaTime)	{


	GJKDetection gjkBoxDetection;
	collisionDetected = gjkBoxDetection.GJKCollisionDetection(mesh1Verts, mesh2Verts);
	
	if (collisionDetected)
		std::cout << "Collision true" << std::endl;
	else
		std::cout << "Collision false" << std::endl;
	Scene::Update(deltaTime);
}

void SceneAi::Render()
{
	Scene::Render();
}

void SceneAi::HandleEvents(const SDL_Event& event)
{
	Scene::HandleEvents(event);
}

void SceneAi::Reset()
{
	OnDestroy();
	OnCreate();
}
