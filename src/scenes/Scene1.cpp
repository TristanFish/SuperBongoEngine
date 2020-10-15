
#include "Scene1.h"
#include "custom/Player.h"
#include "tiles/Tilemap.h"
#include "core/Debug.h"
#include <gl/GLU.h>
#include "custom/MouseRay.h"
#include "core/TextureManager.h"
#include "imgui/imgui_impl_opengl3.h"
Scene1::Scene1()
{}

Scene1::~Scene1()
{
	
}

bool Scene1::OnCreate()
{
	
	std::cout << "scene1 loaded" << std::endl;
	objectList = new Manager();

	texture = Texture();
	texture.LoadImage("resources/textures/pufflet.bmp");
	//Setup the player
	player = new Player("Player", MATH::Vec3(0.0f, 50.0f, 0.0f));

	grass = new Grass("Grass", MATH::Vec3(0.0f, 1.0f, 0.0f), 700);
	plane = new Plane("Plane", MATH::Vec3(0.0f, 0.0f, 0.0f));
	fog = new TestModel("Fog", MATH::Vec3(0.0f, 30.0f, 0.0f));
	mouseRay = new MouseRay();
	selectedMeshColor = new Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	objectList->AddGameObject(player,1);
	objectList->AddGameObject(grass, 2);
	objectList->AddGameObject(plane, 3);
	objectList->AddGameObject(fog, 4);

	//This init function separates any game objects that have rigid bodies for their physics calculations
	objectList->Init();

	//SaveMapData();
	//LoadMapData();
	return false;
}



void Scene1::OnDestroy()
{
	delete objectList;
	objectList = nullptr;

	delete mouseRay;
	mouseRay = nullptr;

	

	pElement = nullptr;

	texture.DestroyTexture();
}

void Scene1::Update(const float deltaTime)
{
	std::cout << player->transform.pos << std::endl;

	//std::cout << 1.0f / deltaTime << std::endl;
	Camera::getInstance()->Update(deltaTime);
	objectList->CheckCollisions();
	objectList->Update(deltaTime);
	mouseRay->Update(deltaTime);
}



void Scene1::Render() const
{
	
	objectList->Render();
	
	if (objectList->Is_Object_Menu_Active)
	{
		static Vec3* Pos_[4];
		// Gets the mesh's properties and then displays them with ImGui
		ImGui::Begin("Properties", &objectList->Is_Object_Menu_Active);
		ImGui::InputText("Mesh Name", (char*)selectedObject->name, IM_ARRAYSIZE(selectedObject->name));
		ImGui::InputFloat3("Position", Vec3(0.0f, 0.0f, 0.0f), 2);
		//ImGui::InputFloat3("Rotation", Rot_);
		//ImGui::InputFloat3("Scale", Scale_);

		//selectedObject->SetPos(Pos_);
		//selectedObject->SetRotation(Rot_);
		//selectedObject->SetScale(Scale_);
		ImGui::ColorEdit4("Mesh Color", (float *)&selectedMeshColor);
		ImGui::End();
	}
	

}

void Scene1::HandleEvents(const SDL_Event& event)
{
	
	mouseRay->HandleEvents(event);
	objectList->HandleEvents(event);

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (CheckIntersection(mouseRay, Camera::getInstance()->getPosition()))
		{
			std::cout << "Mouse Hit -" << selectedObject->name << std::endl;
			objectList->Is_Object_Menu_Active = true;
		}
	}
}

void Scene1::Reset()
{
	OnDestroy();
	OnCreate();
}

void Scene1::SaveMapData()
{
	SaveMapData();
}

void Scene1::LoadMapData()
{
	LoadMapData();

		
}

bool Scene1::CheckIntersection(MouseRay* ray, const Vec3& origin)
{
	
	for (auto obj : objectList->GetGameObjects())
	{
		
		if (obj->objectID == 3)
		{
			Vec3 bounds[2];
			bounds[0] = obj->getComponent<MeshRenderer>().GetMinVector();
			bounds[1] = obj->getComponent<MeshRenderer>().GetMaxVector();
			selectedObject = obj;
			selectedMeshColor = &obj->getComponent<MeshRenderer>().GetMeshColor();

			float tmin, tmax, tymin, tymax, tzmin, tzmax;

			tmin = (bounds[ray->sign[0]].x - origin.x) * ray->GetInvCurrentRay().x;
			tmax = (bounds[1 - ray->sign[0]].x - origin.x) * ray->GetInvCurrentRay().x;
			tymin = (bounds[ray->sign[1]].y - origin.y) * ray->GetInvCurrentRay().y;
			tymax = (bounds[1 - ray->sign[1]].y - origin.y) * ray->GetInvCurrentRay().y;

			if ((tmin > tymax) || (tymin > tmax))
				return false;
			if (tymin > tmin)
				tmin = tymin;
			if (tymax < tmax)
				tmax = tymax;

			tzmin = (bounds[ray->sign[2]].z - origin.z) * ray->GetInvCurrentRay().z;
			tzmax = (bounds[1 - ray->sign[2]].z - origin.z) * ray->GetInvCurrentRay().z;

			if ((tmin > tzmax) || (tzmin > tmax))
				return false;
			if (tzmin > tmin)
				tmin = tzmin;
			if (tzmax < tmax)
				tmax = tzmax;

			return true;
		}
	}
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

	

