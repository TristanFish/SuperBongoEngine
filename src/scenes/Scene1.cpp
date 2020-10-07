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

	texture = Texture();
	texture.LoadImage("resources/textures/pufflet.bmp");
	//Setup the player
	player = new Player("Player", MATH::Vec3(0.0f, 50.0f, 0.0f));

	grass = new Grass("Grass", MATH::Vec3(0.0f, 1.0f, 0.0f), 700);
	plane = new Plane("Plane", MATH::Vec3(0.0f, 0.0f, 0.0f));
	fog = new TestModel("Fog", MATH::Vec3(0.0f, 10.0f, 0.0f));
	objectList->AddGameObject(player,1);
	objectList->AddGameObject(grass, 2);
	objectList->AddGameObject(plane, 3);
	objectList->AddGameObject(fog, 4);

	//This init function separates any gameobjects that have rigidbodies for their physics calculations
	objectList->Init();

	//SaveMapData();
	LoadMapData();
	return false;
}



void Scene1::OnDestroy()
{
	delete objectList;
	objectList = nullptr;
	texture.DestroyTexture();
}

void Scene1::Update(const float deltaTime)
{
	//std::cout << 1.0f / deltaTime << std::endl;
	Camera::getInstance()->Update(deltaTime);
	objectList->CheckCollisions();
	objectList->Update(deltaTime);

}



void Scene1::Render() const
{
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	objectList->Render();
	Debug::DrawTextureToScreen(1, MATH::Vec2(-1.0f, -0.5f), MATH::Vec2(-0.5f, -0.5f), MATH::Vec2(-0.5f, -1.0f), MATH::Vec2(-1.0f, -1.0f));
	
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	
	if (event.key.keysym.sym == SDLK_k)
	{
		TestModel* fog2 = new TestModel("Fog_2", Vec3(0.0, 30.0, 0.0));
		objectList->AddGameObject(fog2,4);
		SaveMapData();
	}


	objectList->HandleEvents(event);
}

void Scene1::Reset()
{
	OnDestroy();
	OnCreate();
}

void Scene1::SaveMapData()
{
	XMLDocument MapData;
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
		XMLElement*  pPosElement = MapData.NewElement("Position");
		XMLElement* pRotElement = MapData.NewElement("Rotation");
		XMLElement* pScaleElement = MapData.NewElement("Scale");

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
	}

	// Once all the gameobjects are looped through it ends the xml file
	pRoot->InsertEndChild(gListElement);

	// Save's the file and checks for succesful save
	XMLError eResult = MapData.SaveFile("MapData.xml");
	if (eResult != XML_SUCCESS)
	{
		std::cout << "Unable to save XML document" << std::endl;
	}
}

void Scene1::LoadMapData()
{
	XMLDocument MapData;
	XMLError eResult = MapData.LoadFile("MapData.xml");

	if (eResult != XML_SUCCESS)
	{
		std::cout <<"Couldn't load the Map File" << std::endl;
	}

	// Get's the root node
	XMLNode* pRoot = MapData.FirstChild();

	int loop = 0;
		XMLElement* pGameObjects = pRoot->FirstChildElement("GameObjects");
		if (pGameObjects == nullptr) { std::cout << "Gameobjects not accesed properly" << std::endl; }
		int objCount = 0;

		eResult = pGameObjects->QueryIntAttribute("Count", &objCount);




		for (XMLElement * pNameElement = pGameObjects->FirstChildElement("Name"); 
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

			// If the save file has more gamobject 's then we have spawned it will spawn 
			// in what is needed depending on the objects IDs
			/*If the current loop is less then or equal to the object count then continue updating values
			  else we want to create a new gameobject*/
			if (loop <= objectList->GetGameObjects().size())
			{
				tempObject = objectList->GetGameObjects()[loop -1];
			}

			if (loop > objectList->GetGameObjects().size())
			{
				std::cout << outID << std::endl;
				const char* objectName;
				eResult = pNameElement->QueryStringAttribute("Equals", &objectName);
				if (outID == 4)
				{
					// Add's a new gamobject to the scene
					tempObject = new TestModel(objectName, Vec3(0.0f,0.0f,0.0f));
					objectList->AddGameObject(tempObject, 4);
					std::cout << "New Object" << std::endl;
				}
			}
		

			


			// Find's child elements of the name
			XMLElement* pPosElement = pNameElement->FirstChildElement("Position");
			XMLElement* pRotElement = pNameElement->FirstChildElement("Rotation");
			XMLElement* pScaleElement = pNameElement->FirstChildElement("Scale");

			// Set's up vectors that we want to save
			Vec3 outPos, outRot,outScale;
			
			// Loops through each Element's attribute's and gets there values
			for (int i = 0; i < 3; i++)
			{
				eResult = pPosElement->QueryFloatAttribute(CheckAtributeValue(i), &outPos[i]);
				eResult = pRotElement->QueryFloatAttribute(CheckAtributeValue(i), &outRot[i]);
				eResult = pScaleElement->QueryFloatAttribute(CheckAtributeValue(i), &outScale[i]);
			}
			
			if (eResult != XML_SUCCESS)
			{
				std::cout << "Unable to set new values for " << tempObject->name << std::endl;
			}
			

			
			tempObject->SetPos(outPos);
			tempObject->SetRotation(outRot);
			tempObject->SetScale(outScale);
		}
		
}

const char* Scene1::CheckAtributeValue(int i)
{
	const char* value = new const char();

	if (i == 0) { value = "X"; }
	if (i == 1) { value = "Y"; }
	if (i == 2) { value = "Z"; }

	return value;
}
