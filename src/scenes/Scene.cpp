#include "Scene.h"
#include "custom/TestModel.h"
#include "custom/Plane.h"
#include "custom/MouseRay.h"


using namespace MATH;

// Returns X,Y,Z Depending on the int it is given
// Used for looking through a vector
 const char* Scene::CheckAtributeValue(int i) const
{
	 const char* value = new char();

	if (i == 0) { value = "X"; }
	if (i == 1) { value = "Y"; }
	if (i == 2) { value = "Z"; }
	if (i == 3) { value = "W"; }
	return value;
}

 void Scene::CreateObjWithID(Vec3 pos_, Vec3 rot_, Vec3 scale_, const char* name_, const int& ID) const
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

 void Scene::CheckExistingPanel(GameObject* obj)
 {
	 for (int i = 0; i < propertiesPannels.size(); i++)
	 {
		 if (propertiesPannels[i]->selectedObj->name == obj->name)
		 {
			 propertiesPannels.erase(propertiesPannels.begin() + i);
		 }
	 }
 }

 bool Scene::CheckIntersection(MouseRay ray, const Vec3& origin, GameObject* obj)
 {
	 Vec3 bounds[2];
	 bounds[0] = obj->getComponent<MeshRenderer>().GetMinVector();


	 bounds[1] = obj->getComponent<MeshRenderer>().GetMaxVector();

	 
			
	 float tx1 = ((bounds[0].x - origin.x) + obj->transform.pos.x) * ray.invDir.x;
	 float tx2 = ((bounds[1].x - origin.x) + obj->transform.pos.x) * ray.invDir.x;
																		
	 float tmin = std::min(tx1, tx2);									
	 float tmax = std::max(tx1, tx2);									
																		
	 float ty1 = ((bounds[0].y - origin.y) + obj->transform.pos.y) * ray.invDir.y;
	 float ty2 = ((bounds[1].y - origin.y) + obj->transform.pos.y) * ray.invDir.y;
																		 
	 tmin = std::max(tmin, std::min(ty1, ty2));							 
	 tmax = std::min(tmax, std::max(ty1, ty2));							 
																		 
	 float tz1 = ((bounds[0].z - origin.z) + obj->transform.pos.z) * ray.invDir.z;
	 float tz2 = ((bounds[1].z - origin.z) + obj->transform.pos.z) * ray.invDir.z;

	 tmin = std::max(tmin, std::min(tz1, tz2));
	 tmax = std::min(tmax, std::max(tz1, tz2));


	 return tmax >= tmin;
 }

 


 void Scene::Update(const float deltaTime)
 {
	 performancePanel.Update(deltaTime);
 }

 void Scene::Render() const
 {
	 ImGui::NewFrame();
	 // Let's the use add game objects

	 for (int i = 0; i < propertiesPannels.size(); i++)
	 {
		 propertiesPannels[i]->Render();
	 }

	 performancePanel.Render();

	 // Displays panel that allows user to add gameobjects at runtime
	 bool enabled = true;
	 static int objID = 2;
	 ImGui::Begin("Add Game Object", &enabled);
	 ImGui::ListBox("Test Level", &objID, objClasses, IM_ARRAYSIZE(objClasses), 2);

	 // Blank variables that can be changed 
	 static char* name_ = new char();
	 static Vec3 Pos_ = Vec3(0.0f);
	 static Vec3 Rot_ = Vec3(0.0f);
	 static Vec3 Scale_ = Vec3(1.0f);


	 if (ImGui::InputText("Mesh Name", name_, size_t(20)))
	 {

	 }
	 ImGui::DragFloat3("Position", Pos_);
	 ImGui::DragFloat3("Rotation", Rot_);
	 ImGui::DragFloat3("Scale", Scale_, -1, 1);
	 if (ImGui::Button("Create Object"))
	 {
		 CreateObjWithID(Pos_, Rot_, Scale_, name_, objID + 3);
		 name_ = new char();
		 Pos_ = Vec3(0.0f);
		 Rot_ = Vec3(0.0f);
		 Scale_ = Vec3(1.0f);
	 }
	 ImGui::End();

 }

 void Scene::HandleEvents(const SDL_Event& event)
 {
	 mouseRay.HandleEvents(event);
	 if (event.type == SDL_MOUSEBUTTONDOWN)
	 {
		 mouseRay.CalaculateMouseRay();
		 for (auto obj : objectList->GetGameObjects())
		 {
			 if (obj->hasComponent<MeshRenderer>())
			 {
				if (CheckIntersection(mouseRay, mouseRay.GetCurrentRay().Origin, obj))
				{
					EngineLogger::Info("Mouse hit " + std::string(obj->name), "Scene.cpp", __LINE__);
					if (!obj->isMenuActive)
					{
						CheckExistingPanel(obj);
						propertiesPannels.push_back(new CustomUI::PropertiesPanel(obj));
					}
					obj->isMenuActive = true;
					return;
				}
			 }
		 }
	 }
 }

 void Scene::SaveMapData() const
{
	XMLDoc MapData;
	XMLNode* pRoot = MapData.NewElement("Root");
	MapData.InsertFirstChild(pRoot);

	// Create a base Element that holds the gameobjects
	XMLElement* gListElement;
	gListElement = MapData.NewElement("GameObjects");
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

		pIDElement->SetAttribute("ID", obj->objectID);
		pNameElement->InsertEndChild(pIDElement);

		// Loops through a vector for each
		for (int i = 0; i < 3; i++)
		{
			pPosElement->SetAttribute(CheckAtributeValue(i), obj->transform.pos[i]);
			pNameElement->InsertEndChild(pPosElement);

			pRotElement->SetAttribute(CheckAtributeValue(i), obj->transform.rotation[i]);
			pNameElement->InsertEndChild(pRotElement);

			pScaleElement->SetAttribute(CheckAtributeValue(i), obj->transform.scale[i]);
			pNameElement->InsertEndChild(pScaleElement);
		}

		if (obj->hasComponent<MeshRenderer>())
		{
			for (int i = 0; i < 4; i++)
			{
				float nextValue = ceil(obj->getComponent<MeshRenderer>().meshColorTint[i] * 255);

				pColorElement->SetAttribute(CheckAtributeValue(i), nextValue);
				pNameElement->InsertEndChild(pColorElement);
			}
		}
	}

	// Once all the gameobjects are looped through it ends the xml file
	pRoot->InsertEndChild(gListElement);

	// Save's the file and checks for successful save
	XMLError eResult = MapData.SaveFile("MapData.xml");
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

	int loop = 0;
	XMLElement* pGameObjects;
	pGameObjects = pRoot->FirstChildElement("GameObjects");
	if (pGameObjects == nullptr) { EngineLogger::Error("GameObjects not accessed properly", "Scene.cpp", __LINE__); }
	int objCount = 0;





	for (XMLElement* pNameElement = pGameObjects->FirstChildElement("Name");
		pNameElement != nullptr;
		pNameElement = pNameElement->NextSiblingElement("Name"))
	{

		GameObject* tempObject = nullptr;
		int outID = 0;
		XMLElement* pIDElement;
		loop++;

		// Gets the gamobjects ID and sets it to the "outID"
		pIDElement = pNameElement->FirstChildElement("Object");
		eResult = pIDElement->QueryIntAttribute("ID", &outID);

		// If the save file has more gamobjects then we have spawned it will spawn 
		// in what is needed depending on the objects IDs
		/*If the current loop is less then or equal to the object count then continue updating values
		  else we want to create a new gameobject*/
		if (loop <= objectList->GetGameObjects().size())
		{
			tempObject = objectList->GetGameObjects()[loop - 1];
		}

		if (loop > objectList->GetGameObjects().size())
		{

			const char* objectName;
			eResult = pNameElement->QueryStringAttribute("Equals", &objectName);
			if (outID == 4)
			{
				// Add's a new gamobject to the scene
				tempObject = new TestModel(objectName, Vec3(0.0f, 0.0f, 0.0f));
				objectList->AddGameObject(tempObject, 4);

			}
			if (outID == 3)
			{
				// Add's a new gamobject to the scene
				tempObject = new Plane(objectName, Vec3(0.0f, 0.0f, 0.0f));
				objectList->AddGameObject(tempObject, 4);

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
			eResult = pPosElement->QueryFloatAttribute(CheckAtributeValue(i), &outPos[i]);
			eResult = pRotElement->QueryFloatAttribute(CheckAtributeValue(i), &outRot[i]);
			eResult = pScaleElement->QueryFloatAttribute(CheckAtributeValue(i), &outScale[i]);
		}


		// Used for loading in Mesh Colors
		if (tempObject->hasComponent<MeshRenderer>())
		{
			MATH::Vec4 outColor;

			XMLElement* pColorElement = pNameElement->FirstChildElement("Color");

			for (int i = 0; i < 4; i++)
			{
				eResult = pColorElement->QueryFloatAttribute(CheckAtributeValue(i), &outColor[i]);
			}

			outColor /= 255.0f;

			tempObject->getComponent<MeshRenderer>().SetColorTint(outColor);
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

