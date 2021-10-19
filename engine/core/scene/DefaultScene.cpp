
#include "DefaultScene.h"

#include "core/MouseRay.h"
#include "core/Logger.h"
#include "core/resources/SaveManager.h"
#include "core/Globals.h"

#include "gameObjects/LightObject.h"
#include "gameObjects/Bird.h"
#include "gameObjects/Player.h"
#include "gameObjects/Grass.h"

#include "math/MMath.h"
#include "Rendering/Camera.h"
#include "Primitives/PlaneObject.h"



DefaultScene::DefaultScene(std::string Scene_Name_) 
{
	Scene_Name = Scene_Name_;
}

DefaultScene::DefaultScene()
{
	Scene_Name = "Default";
}

bool DefaultScene::OnCreate()
{
	EngineLogger::Info(Scene_Name + " Created", "DefaultScene.cpp", __LINE__);


	Globals::InitGlobals();

	LoadMapData();


	objectList->Init();



	return true;
}

void DefaultScene::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
	Camera::getInstance()->Update(deltaTime);
	


	GameObject* tet_1 = &objectList->FindGameObject("Tet_1");
	GameObject* tet_2 = &objectList->FindGameObject("Tet_2");
	int loops = 0;
	/* Get two pointers to our tetrahedrons mesh renderers so that 
	we can then extract the vertices's and convert them to world space*/
	MeshRenderer* renderer_1 = tet_1->GetComponent<MeshRenderer>();
	MeshRenderer* renderer_2 = tet_2->GetComponent<MeshRenderer>();

	/*Loop through tetrahedron 1's vertices's, calculate the vertex's world position , 
	loop through tetrahedron 2's vertices's, minus tetrahedron 1's vertex world 
	position by tetrahedron 2's vertex world position, Print out value  */
	for (const auto& tet_Vert1 : renderer_1->GetModel()->GetVerticies())
	{
		MATH::Vec3 newPos = tet_1->transform.GetModelMatrix() * tet_Vert1.position;

		for (const auto& tet_Vert2 : renderer_2->GetModel()->GetVerticies())
		{
			MATH::Vec3 posToPrint = newPos - (tet_2->GetModelMatrix() * tet_Vert2.position);
				
			posToPrint.print();
		}
		
		loops++;
		std::cout << "------FINISHED " << loops << "------" << std::endl;
	}
}

void DefaultScene::Render()
{
	Scene::Render();
}

void DefaultScene::HandleEvents(const SDL_Event& event)
{
	Scene::HandleEvents(event);
}

void DefaultScene::Reset()
{
	OnDestroy();
	OnCreate();
}