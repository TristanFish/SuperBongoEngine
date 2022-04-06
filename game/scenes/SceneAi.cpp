#include "SceneAi.h"


#include "Scene1.h"
#include "core/Logger.h"
#include "core/Globals.h"
#include "gameObjects/AiGameObject.h"
#include "Primitives/Sphere.h"
#include "Primitives/Box.h"


SceneAi::SceneAi()
{
	Scene_Name = "Scene_AI";
}

bool SceneAi::OnCreate()
{
	EngineLogger::Info("Scene Ai Created", "SceneAi.cpp", __LINE__);

	std::shared_ptr<AiGameObject> obj = std::make_shared<AiGameObject>("ai object", Vec3());
	objectList->AddGameObject(obj);
	
	///*AiGameObject* mainAi = new AiGameObject("AiObj",Vec3(1.0f));
	//objectList->AddGameObject(mainAi);

	//Sphere* targetSphere = new Sphere("targetSphere",Vec3(15.0f, 2.5f, 7.0f));
	//objectList->AddGameObject(targetSphere);
	//
	//mainAi->aiTarget = targetSphere;*/

	////GJK Code test;
	//Box* box1 = new Box("Box1", Vec3(25.0f, 0.0f, 0.0f));
	//
	////mesh1 = new Mesh(box1->GetComponent<MeshRenderer>()->GetMeshes().front());
	////for (auto vertex : mesh1->vertices) {

	////	vertex.position = vertex.position + Vec3(25.0f, 0.0f, 0.0f);
	////	//std::cout << vertex.position << std::endl;
	////	int test = 0;
	////}

	//objectList->AddGameObject(box1);

	//Box* box2 = new Box("Box2", Vec3(-25.0f, 0.0f, 0.0f));
	///*mesh2 = new Mesh(box2->GetComponent<MeshRenderer>()->GetMeshes().front());
	//for (auto vertex : mesh1->vertices) {
	//	vertex.position += Vec3(-25.0f, 0.0f, 0.0f);
	//	std::cout << vertex.position << std::endl;
	//}*/
	//objectList->AddGameObject(box2);

	//
	//{
	//	mesh1Verts.emplace_back(Vec3(0.5f, 0.8f, 0.3f));
	//	mesh1Verts.emplace_back(Vec3(0.0f));
	//	mesh1Verts.emplace_back(Vec3(0.5f, 0.0f, 0.9f));
	//	mesh1Verts.emplace_back(Vec3(1.0f, 0.0f, 0.0f));
	//	
	//}

	////original 2nd tetra from example 
	///*{
	//	mesh2Verts.emplace_back(Vec3(0.5f, 0.1f, 0.3f));
	//	mesh2Verts.emplace_back(Vec3(0.0f, -0.7f, 0.0f));
	//	mesh2Verts.emplace_back(Vec3(0.5f, -0.7f, 0.9f));
	//	mesh2Verts.emplace_back(Vec3(1.0f, -0.7f, 0.0f));
	//}*/

	////modified to be a bit more random shape
	//{
	//	mesh2Verts.emplace_back(Vec3(1.0f, 1.0f, 1.0f));
	//	mesh2Verts.emplace_back(Vec3(0.0f, -1.0f, 1.0f));
	//	mesh2Verts.emplace_back(Vec3(1.0f, -1.7f, 1.9f));
	//	mesh2Verts.emplace_back(Vec3(1.0f, -1.7f, 0.0f));
	//}
	////mesh1 starts to the right
	//for (Vec3& point : mesh1Verts) {
	//	point.x = point.x + 5.0f;
	//}
	////mesh2 starts to the left
	//for (Vec3& point : mesh2Verts) {
	//	point.x = point.x - 5.0f;
	//}

	objectList->Init();

	
	return true;
}

void SceneAi::Update(const float deltaTime)	
{
	//GJKDetection gjkBoxDetection;
	//collisionDetected = gjkBoxDetection.GJKCollisionDetection(mesh1Verts, mesh2Verts);
	//
	//if (collisionDetected)
	//	std::cout << "Collision true" << std::endl;
	//else
	//	std::cout << "Collision false" << std::endl;

	////moves left
	//for(Vec3& point : mesh1Verts)	{
	//	point.x = point.x - 1.0f;
	//}
	////moves right
	//for (Vec3& point : mesh2Verts) {
	//	point.x = point.x + 1.0f;
	//}
	
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
