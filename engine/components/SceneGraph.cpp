#include "SceneGraph.h"
#include "GameObject.h"
#include "core/Logger.h"
//#include "core/3D/Physics3D.h"
#include "core/Globals.h"
#include "core/resources/SaveFile.h"
#include "../game/gameObjects/Grass.h"
#include "../game/gameObjects/LightObject.h"
#include "core/resources/SaveManager.h"

#include "graphics/UIStatics.h"


SceneGraph::~SceneGraph() 
{
	for (GameObject* g : gameObjects)
	{
		if (g)
		{
			delete g;
			g = nullptr;
		}
	}

	for (auto nameObjectPair : InstantiableObjects)
	{
		delete nameObjectPair.second;
		nameObjectPair.second = nullptr;
	}

	InstantiableObjects.clear();
	
	gameObjects.clear();

	rigidBodies.clear();
}

void SceneGraph::Init() 
{
	//ScenePartition = new OctSpatialPartition(1500);

	for (const auto& obj : SaveManager::SaveableObjects)
	{
		//if obj is not already in InstantiableObjects
		if(InstantiableObjects.find(obj.first) == InstantiableObjects.end())
		{
			if (obj.second->canBeInstantiated)
			{
				InstantiableObjects.emplace(obj.first, obj.second->NewClone());
			}
		}
	}

	for(auto& go : gameObjects)
	{
		go->Init();
	}
}

void SceneGraph::PostInit()
{
	for(GameObject* go : gameObjects)
	{
		go->PostInit();
	}
	sceneIsPostInit = true;
}

void SceneGraph::Update(const float deltaTime)
{
	for (auto* g : gameObjects)
	{
		if (g->isActive())
		{
			g->Update(deltaTime);
		}
	}
}
void SceneGraph::Render() const
{
	//Clear the default framebuffer
	Renderer::GetInstance()->defaultBuffer.Bind();
	Renderer::GetInstance()->defaultBuffer.Clear();
	//Bind the gbuffer and clear it
	Renderer::GetInstance()->gBuffer.Bind();
	Renderer::GetInstance()->gBuffer.Clear();
	glEnable(GL_DEPTH_TEST);

	Renderer::GetInstance()->Render();
}

void SceneGraph::HandleEvents(const SDL_Event& event)
{
	for (auto* g : gameObjects)
	{
		g->HandleEvents(event);
	}
}

//Finds THE FIRST gameObject with the given name
GameObject* SceneGraph::FindGameObject(const std::string& name)
{
	for (auto* g : gameObjects)
	{
		if (g->name == name)
		{
			return g;
		}
	}

	//EngineLogger::Error(std::string("No object named \"" + std::string(name) + "\" was found."), "SceneGraph.cpp", __LINE__);

	return nullptr;
}

void SceneGraph::GameObjectNetworkUpdate(std::string& string)
{
	NetworkableObject tmp = NetworkableObject();
	std::stringstream ss(string);
	std::stringstream name = tmp.FindGameObjectName(string);
	tmp = *FindGameObject(name.str().c_str())->GetComponent<NetworkableObject>();
	tmp.RecievePositionData(ss);

}

//Adds a gameObject with a name and position
GameObject& SceneGraph::AddGameObject(GameObject* go)
{
	if (!FindGameObject(go->name.c_str()))
	{
		LoadGameObject(go);
	}
	else
	{
		go->name += "_" + std::to_string(1);
		LoadGameObject(go);
	}
	
	return *go;
}

void SceneGraph::LoadGameObject(GameObject* go)
{
	go->Init();

	gameObjects.emplace_back(go);
	EngineLogger::Info(std::string(go->name) + " added to scenegraph", "ECS.cpp", __LINE__);

	if(sceneIsPostInit)
	{
		//if the scene has already finished its OnCreate function make sure to call PostInit
		go->PostInit();
	}

	if (go->HasComponent<RigidBody3D>())
	{
		RigidBody3D* rb = go->GetComponent<RigidBody3D>();
		rigidBodies.emplace_back(rb);
		//ScenePartition->AddObject(rb->GetCollider());
	}

	for (GameObject* child : go->children)
	{
		AddGameObject(child);
	}
}

void SceneGraph::DeleteGameObject(GameObject* go)
{
	if (go->HasComponent<RigidBody3D>())
	{
		for (std::vector<RigidBody3D*>::iterator iter = rigidBodies.begin(); iter != rigidBodies.end(); iter++)
		{
			if (*iter == go->GetComponent<RigidBody3D>())
			{
				rigidBodies.erase(iter);
				break;
			}
		}
	}

	for (std::vector<GameObject*>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		if (*iter == go)
		{
			gameObjects.erase(iter);
			delete go;
			go = nullptr;
			break;
		}
	}
}

const std::unordered_map<std::string, GameObject*>& SceneGraph::GetInstantiableObjects() const
{
	return InstantiableObjects;
}

void SceneGraph::CheckCollisions()
{
	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		for (size_t j = i + 1; j < rigidBodies.size(); j++)
		{
			//Physics3D::DetectCollision(*rigidBodies[i], *rigidBodies[j]);
		}
	}
}
