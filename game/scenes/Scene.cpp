#include "Scene.h"
#include "gameObjects/TestModel.h"
#include "Primitives/PlaneObject.h"
#include <math.h>
#include "core/MouseRay.h"
#include "core/Logger.h"
#include "core/3D/Physics3D.h"

using namespace MATH;

// Returns X,Y,Z Depending on the int it is given
// Used for looking through a vector
std::string Scene::CheckAtributeValue(int i) const
{
	std::string value;

	if (i == 0) { value = 'X'; }
	if (i == 1) { value = 'Y'; }
	if (i == 2) { value = 'Z'; }
	if (i == 3) { value = 'W'; }
	return value;
}

void Scene::OnMouseMove(MATH::Vec2 mouse)
{
	
}

void Scene::OnMousePressed(MATH::Vec2 mouse, int buttonType)
{
	if (buttonType == SDL_BUTTON_LEFT)
	{
		mouseRay.CalaculateMouseRay();

		//GameObject* hitResult = objectList->osp.GetCollision(mouseRay);
		GameObject* hitResult = nullptr;
		float shortestDistance = FLT_MAX;
		for (auto* obj : objectList->GetGameObjects())
		{
			if (obj->HasComponent<MeshRenderer>())
			{
				MeshRenderer* mr = obj->GetComponent<MeshRenderer>();
				if (Physics3D::RayOBBDetect(mouseRay, mr->OBB))//(CheckIntersection(mouseRay, mouseRay.GetCurrentRay().Origin, obj))
				{
					if(mouseRay.intersectionDist < shortestDistance)
					{
						hitResult = obj;
						shortestDistance = mouseRay.intersectionDist;
					}
				}
			}
		}

		if(hitResult)
		{
			EngineLogger::Info("Mouse hit " + std::string(hitResult->name), "Scene.cpp", __LINE__);
			if (!hitResult->isMenuActive)
			{
				CheckExistingPanel(hitResult);
				propertiesPanels.push_back(new CustomUI::PropertiesPanel(hitResult));
			}
			hitResult->isMenuActive = true;
		}
	}
}

void Scene::CreateObjWithID(const Vec3& pos_, const Vec3& rot_, const Vec3& scale_, const char* objName_, const char* IDName) const
{
	if(strcmp(IDName, "PlaneObject") == 0)
	{
		PlaneObject* newPlane_ = new PlaneObject(name_, pos_);
		newPlane_->SetRotation(rot_);
		newPlane_->SetScale(scale_);
		objectList->AddGameObject(newPlane_);
		return;
	}
	else if(strcmp(IDName,"TestModel") == 0)
	{
		TestModel* newTestModel = new TestModel(name_, pos_);
		newTestModel->SetRotation(rot_);
		newTestModel->SetScale(scale_);
		objectList->AddGameObject(newTestModel);
		return;
	}
	EngineLogger::Warning("ID \"" + std::string(IDName) + "\" was not found","Scene.cpp", __LINE__);
}

 void Scene::CheckExistingPanel(GameObject* obj)
 {
	 for (size_t i = 0; i < propertiesPanels.size(); i++)
	 {
		 if (propertiesPanels[i]->selectedObj->name == obj->name)
		 {
			 propertiesPanels.erase(propertiesPanels.begin() + i);
		 }
	 }
 }

bool Scene::CheckIntersection(const MouseRay& ray, const Vec3& origin, GameObject* obj) const
{
	Vec3 bounds[2];
	bounds[0] = obj->GetComponent<MeshRenderer>()->GetMinVector();


	bounds[1] = obj->GetComponent<MeshRenderer>()->GetMaxVector();

			
	const float tx1 = ((bounds[0].x - origin.x) + obj->transform.pos.x) * ray.invDir.x;
	const float tx2 = ((bounds[1].x - origin.x) + obj->transform.pos.x) * ray.invDir.x;
																		
	float tmin = std::min(tx1, tx2);									
	float tmax = std::max(tx1, tx2);									
																		
	const float ty1 = ((bounds[0].y - origin.y) + obj->transform.pos.y) * ray.invDir.y;
	const float ty2 = ((bounds[1].y - origin.y) + obj->transform.pos.y) * ray.invDir.y;
																		 
	tmin = std::max(tmin, std::min(ty1, ty2));							 
	tmax = std::min(tmax, std::max(ty1, ty2));							 
																		 
	const float tz1 = ((bounds[0].z - origin.z) + obj->transform.pos.z) * ray.invDir.z;
	const float tz2 = ((bounds[1].z - origin.z) + obj->transform.pos.z) * ray.invDir.z;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));


	return tmax >= tmin;
}


Scene::Scene() : name_(new char()), objectList(nullptr), pElement(nullptr)
{
	objectList = std::make_unique<SceneGraph>();

	
}

Scene::~Scene()
{

	if(name_)
	{
		delete name_;
		name_ = nullptr;
	}

	for(auto panel : propertiesPanels)
	{
		if(panel)
		{
			delete panel;
			panel = nullptr;
		}
	}
	propertiesPanels.clear();
}

bool Scene::PostCreate()
{
	std::vector<GameObject*> gameObjects;
	
	for (auto object : objectList->GetGameObjects())
	{
		if(object->GetParent() == nullptr)
		{
			gameObjects.push_back(object);
		}
	}
	
	hierarchyPanel.ConstructHierarchy(gameObjects);

	return true;
}

void Scene::Update(const float deltaTime)
{
	objectList->Update(deltaTime);
	objectList->CheckCollisions();
	performancePanel.Update(deltaTime);
}

void Scene::Render() const
{
	ImGui::NewFrame();
	// Let's the use add game objects

	for (size_t i = 0; i < propertiesPanels.size(); i++)
	{
		propertiesPanels[i]->Render();
	}

	 performancePanel.Render();
	 hierarchyPanel.Render();

	 // Displays panel that allows user to add gameobjects at runtime
	 bool enabled = true;
	 static int objID = 0;
	 ImGui::Begin("Add Game Object", &enabled);
	 ImGui::ListBox("Test Level", &objID, objClasses, IM_ARRAYSIZE(objClasses), 3);

	// Blank variables that can be changed 
	static Vec3 Pos_ = Vec3(0.0f);
	static Vec3 Rot_ = Vec3(0.0f);
	static Vec3 Scale_ = Vec3(1.0f);


	if (ImGui::InputText("Mesh Name", name_, 20))
	{


	}

	ImGui::DragFloat3("Position", Pos_);
	ImGui::DragFloat3("Rotation", Rot_);
	ImGui::DragFloat3("Scale", Scale_, -1, 1);
	
	if (ImGui::Button("Create Object"))
	{
		CreateObjWithID(Pos_, Rot_, Scale_, name_, objClasses[objID]);
		Pos_ = Vec3(0.0f);
		Rot_ = Vec3(0.0f);
		Scale_ = Vec3(1.0f);
	}
	ImGui::End();

	objectList->Render();
}


void Scene::HandleEvents(const SDL_Event& event)
{
	mouseRay.HandleEvents(event);
	objectList->HandleEvents(event);
}

void Scene::SaveMapData() const
{
	XMLDoc MapData;
	XMLNode* pRoot = MapData.NewElement("Root");
	MapData.InsertFirstChild(pRoot);

	// Create a base Element that holds the gameobjects
	XMLElement* gListElement = MapData.NewElement("GameObjects");
	int ObjAmount = 0;

	pRoot->InsertFirstChild(gListElement);
	for (const auto& obj : objectList->GetGameObjects())
	{
		ObjAmount++;
		gListElement->SetAttribute("Count", ObjAmount);

		// Create's a new element that will hold the objects name
		XMLElement* pNameElement = MapData.NewElement("Name");
		pNameElement->SetAttribute("Equals", obj->name);
		gListElement->InsertEndChild(pNameElement);


		// Create's Element's to hold the information we want to save
		XMLElement* pIDElement = MapData.NewElement("Object");
		XMLElement* pPosElement = MapData.NewElement("Position");
		XMLElement* pRotElement = MapData.NewElement("Rotation");
		XMLElement* pScaleElement = MapData.NewElement("Scale");
		XMLElement* pColorElement = MapData.NewElement("Color");


		pIDElement->SetAttribute("ID", obj->GetType());

		pNameElement->InsertEndChild(pIDElement);

		// Loops through a vector for each
		for (int i = 0; i < 3; i++)
		{
			pPosElement->SetAttribute(CheckAtributeValue(i).c_str(), obj->transform.pos[i]);
			pNameElement->InsertEndChild(pPosElement);

			pRotElement->SetAttribute(CheckAtributeValue(i).c_str(), obj->transform.rotation[i]);
			pNameElement->InsertEndChild(pRotElement);

			pScaleElement->SetAttribute(CheckAtributeValue(i).c_str(), obj->transform.scale[i]);
			pNameElement->InsertEndChild(pScaleElement);
		}

		if (obj->HasComponent<MeshRenderer>())
		{
			for (int i = 0; i < 4; i++)
			{
				const float nextValue = ceil(obj->GetComponent<MeshRenderer>()->meshColorTint[i] * 255);

				pColorElement->SetAttribute(CheckAtributeValue(i).c_str(), nextValue);
				pNameElement->InsertEndChild(pColorElement);
			}
		}
	}

	// Once all the gameobjects are looped through it ends the xml file
	pRoot->InsertEndChild(gListElement);

	// Save's the file and checks for successful save
	const XMLError eResult = MapData.SaveFile("MapData.xml");
	if (eResult != XML_SUCCESS)
	{
		EngineLogger::Error("Unable to save XML document", "Scene.cpp", __LINE__);
	}
}

void Scene::LoadMapData()
{
	XMLDoc MapData;
	XMLError eResult = MapData.LoadFile("MapData.xml");

	if (eResult != XML_SUCCESS)
	{
		EngineLogger::Error("Couldn't load the Map File", "Scene.cpp", __LINE__);
	}

	// Get's the root node
	XMLNode* pRoot = MapData.FirstChild();

	size_t loop = 0;
	XMLElement* pGameObjects;
	pGameObjects = pRoot->FirstChildElement("GameObjects");
	if (pGameObjects == nullptr) { EngineLogger::Error("GameObjects not accessed properly", "Scene.cpp", __LINE__); }
	int objCount = 0;





	for (XMLElement* pNameElement = pGameObjects->FirstChildElement("Name");
		pNameElement != nullptr;
		pNameElement = pNameElement->NextSiblingElement("Name"))
	{

		GameObject* tempObject = nullptr;
		const char* outID = "gy";
		XMLElement* pIDElement;
		loop++;

		// Gets the gamobjects ID and sets it to the "outID"
		pIDElement = pNameElement->FirstChildElement("Object");
		eResult = pIDElement->QueryAttribute("ID", &outID);//  QueryIntAttribute("ID", outID);

		// If the save file has more gamobjects then we have spawned it will spawn 
		// in what is needed depending on the objects IDs
		/*If the current loop is less then or equal to the object count then continue updating values
		  else we want to create a new gameobject*/
		if (loop <= objectList->GetGameObjects().size())
		{
			tempObject = objectList->GetGameObjects()[loop - 1];
		}

		// ID LOADING NEEDS TO BE FIXED
		if (loop > objectList->GetGameObjects().size())
		{

			const char* objectName;
			eResult = pNameElement->QueryStringAttribute("Equals", &objectName);

			if (strcmp(outID, "TestModel") == 0)
			{
				// Add's a new gamobject to the scene
				tempObject = new TestModel(objectName, Vec3(0.0f, 0.0f, 0.0f));
				objectList->AddGameObject(tempObject);

			}
			if (strcmp(outID, "PlaneObject") == 0)
			{
				// Add's a new gamobject to the scene
				tempObject = new PlaneObject(objectName, Vec3(0.0f, 0.0f, 0.0f));
				objectList->AddGameObject(tempObject);

			}
		}


		// Find's child elements of the name
		XMLElement* pPosElement = pNameElement->FirstChildElement("Position");
		XMLElement* pRotElement = pNameElement->FirstChildElement("Rotation");
		XMLElement* pScaleElement = pNameElement->FirstChildElement("Scale");

		// Set's up vectors that we want to save
		MATH::Vec3 outPos, outRot, outScale;

		// Loops through each Element's attribute's and gets there values
		for (int i = 0; i < 3; i++)
		{
			eResult = pPosElement->QueryFloatAttribute(CheckAtributeValue(i).c_str(), &outPos[i]);
			eResult = pRotElement->QueryFloatAttribute(CheckAtributeValue(i).c_str(), &outRot[i]);
			eResult = pScaleElement->QueryFloatAttribute(CheckAtributeValue(i).c_str(), &outScale[i]);
		}


		// Used for loading in Mesh Colors
		if (tempObject->HasComponent<MeshRenderer>())
		{
			MATH::Vec4 outColor;

			XMLElement* pColorElement = pNameElement->FirstChildElement("Color");

			for (int i = 0; i < 4; i++)
			{
				eResult = pColorElement->QueryFloatAttribute(CheckAtributeValue(i).c_str(), &outColor[i]);
			}

			outColor /= 255.0f;

			tempObject->GetComponent<MeshRenderer>()->SetColorTint(outColor);
		}

		if (eResult != XML_SUCCESS)
		{
			EngineLogger::Error("Unable to set new values for " + std::string(tempObject->name), "Scene.cpp", __LINE__);
		}

		tempObject->SetPos(outPos);
		tempObject->SetRotation(outRot);
		tempObject->SetScale(outScale);
	}
}