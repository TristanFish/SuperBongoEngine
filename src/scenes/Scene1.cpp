
#include "Scene1.h"
#include "custom/Player.h"
#include "tiles/Tilemap.h"
#include "core/Debug.h"
#include "custom/MouseRay.h"
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

	grass = new Grass("Grass", MATH::Vec3(0.0f, 4.0f, 0.0f), 700);
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
	
	//Scene::SaveMapData();
	//LoadMapData();
	return false;
}



void Scene1::OnDestroy()
{
	delete mouseRay;
	delete objectList;
	objectList = nullptr;

	delete mouseRay;
	mouseRay = nullptr;

	

	pElement = nullptr;

}

void Scene1::Update(const float deltaTime)
{
	std::cout << player->transform.pos << std::endl;

	//std::cout << 1.0f / deltaTime << std::endl;
	Camera::getInstance()->Update(deltaTime);
	objectList->CheckCollisions();
	objectList->Update(deltaTime);

}



void Scene1::Render() const
{
	ImGui::NewFrame();
	objectList->Render();
	ImGui::SetItemDefaultFocus();

	// Displays a gameobject information
	if (objectList->Is_Object_Menu_Active)
	{
		// Gets the mesh's properties and then displays them with ImGui
		ImGui::Begin("Properties", &objectList->Is_Object_Menu_Active);
		
		char* TempName = new char(); 
		TempName = (char*)selectedObject->name;
		if(ImGui::InputText("Mesh Name", TempName, size_t(20)))
		{	selectedObject->name = TempName;    }
		
		// Change the standard transform components 
		ImGui::DragFloat3("Position", selectedObject->transform.GetPosition());
		ImGui::DragFloat3("Rotation", selectedObject->transform.GetRotation());
		ImGui::DragFloat3("Scale", selectedObject->transform.GetScale(),-1,1);

		// Create a new color that is a copy of the meshes color
		static Vec4 Color_ = selectedObject->getComponent<MeshRenderer>().meshColorTint;
		ImGui::ColorEdit4("Mesh Color", (float *)&Color_);
		
		//
		selectedObject->getComponent<MeshRenderer>().meshColorTint = Color_;
		if (ImGui::Button("Save"))
		{
			SaveMapData();
		}

		ImGui::End();
	}
	
	// Let's the use add game objects

	bool enabled = true;
	

	static int objID = 2;
	ImGui::Begin("Add Game Object", &enabled);
	ImGui::ListBox("Test Level", &objID, objClasses, IM_ARRAYSIZE(objClasses), 2);
	
	// Change the standard transform components 

	 static char* name_ = new char();
	static Vec3 Pos_ = Vec3(0.0f);
	static Vec3 Rot_ = Vec3(0.0f);
	static Vec3 Scale_ = Vec3(1.0f);
	ImGui::InputText("Mesh Name", name_, size_t(20));
	ImGui::DragFloat3("Position", Pos_);
	ImGui::DragFloat3("Rotation", Rot_);
	ImGui::DragFloat3("Scale", Scale_, -1, 1);
	if (ImGui::Button("Create Object"))
	{
		CreateObjWithID(Pos_, Rot_, Scale_, name_, objID + 3);
	}
	ImGui::End();

}

void Scene1::HandleEvents(const SDL_Event& event)
{
	
	mouseRay->HandleEvents(event);
	objectList->HandleEvents(event);

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		mouseRay->CalaculateMouseRay();

		for (int i = 0; i < objectList->GetNumObjects(); i++)
		{
			if (objectList->GetGameObjects()[i]->hasComponent<MeshRenderer>())
			{
				if (CheckIntersection(mouseRay, Camera::getInstance()->getPosition(), objectList->GetGameObjects()[i]))
				{
					std::cout << "Mouse Hit - " << selectedObject->name << std::endl;
					objectList->Is_Object_Menu_Active = true;
				}
			}
		}
	}
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

bool Scene1::CheckIntersection(MouseRay* ray, const Vec3& origin, GameObject* obj)
{
	Vec3 bounds[2];
	bounds[0] = obj->getComponent<MeshRenderer>().GetMinVector();
	

	bounds[1] = obj->getComponent<MeshRenderer>().GetMaxVector();
	selectedObject = obj;
			
	float tx1 = (bounds[0].x - origin.x) * ray->invDir.x;
	float tx2 = (bounds[1].x - origin.x) * ray->invDir.x;

	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

	float ty1 = (bounds[0].y - origin.y) * ray->invDir.y;
	float ty2 = (bounds[1].y - origin.y) * ray->invDir.y;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	float tz1 = (bounds[0].z - origin.z) * ray->invDir.z;
	float tz2 = (bounds[1].z - origin.z) * ray->invDir.z;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));


	return tmax >= tmin;
}

void Scene1::CreateObjWithID(Vec3 pos_, Vec3 rot_, Vec3 scale_, const char* name_, const int& ID) const
{
	switch (ID)
	{
	case 3:
		Plane * newPlane_;
		newPlane_ = new Plane(name_, pos_);
		newPlane_->SetRotation(rot_);
		newPlane_->SetScale(scale_);
		objectList->AddGameObject(newPlane_, ID);
		break;
	case 4:
		TestModel * newTestModel;
		newTestModel = new TestModel(name_, pos_);
		newTestModel->SetRotation(rot_);
		newTestModel->SetScale(scale_);
		objectList->AddGameObject(newTestModel, ID);
		break;
	default:
		break;
	}

}


