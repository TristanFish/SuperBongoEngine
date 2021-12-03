#include "SceneGraph.h"
#include "GameObject.h"

#include "core/Logger.h"
#include "core/3D/OctSpatialPartition.h"
#include "core/Globals.h"
#include "core/resources/SaveFile.h"
#include "core/resources/CollisionDetection.h"
#include "core/resources/SaveManager.h"

#include "../game/gameObjects/Grass.h"
#include "../game/gameObjects/LightObject.h"

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

	delete ScenePartition;
	ScenePartition = nullptr;

	InstantiableObjects.clear();
	
	gameObjects.clear();

	rigidBodies.clear();
}

void SceneGraph::Init() 
{
	ScenePartition = new OctSpatialPartition(500);

	
	
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
GameObject* SceneGraph::FindGameObject(const char* name)
{
	for (auto* g : gameObjects)
	{
		if (g->name == name)
		{
			return g;
		}
	}

	EngineLogger::Error(std::string("No object named \"" + std::string(name) + "\" was found."), "SceneGraph.cpp", __LINE__);

	return nullptr;
}

//Adds a gameObject with a name and position
GameObject& SceneGraph::AddGameObject(GameObject* go)
{

	if (!isObjectActive(go->name))
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

void SceneGraph::AddRenderingComponents()
{
	for (auto* go : gameObjects)
	{
		if (go->HasComponent<MeshRenderer>())
		{
			MeshRenderer* mr = go->GetComponent<MeshRenderer>();
			Renderer::GetInstance()->AddMeshRenderer(mr);
		}
		if (go->HasComponent<LightComponent>())
		{
			Renderer::GetInstance()->AddLight(go->GetComponent<LightComponent>());
		}
	}
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
		ScenePartition->AddObject(rb->GetCollider());
	}
	if (go->HasComponent<MeshRenderer>())
	{
		MeshRenderer* mr = go->GetComponent<MeshRenderer>();
		Renderer::GetInstance()->AddMeshRenderer(mr);
		//osp.AddObject(mr);
	}
	if (go->HasComponent<LightComponent>())
	{
		Renderer::GetInstance()->AddLight(go->GetComponent<LightComponent>());
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
	if (go->HasComponent<MeshRenderer>())
	{
		MeshRenderer* mr = go->GetComponent<MeshRenderer>();
		Renderer::GetInstance()->DeleteMeshRenderer(mr);
		
	}
	if (go->HasComponent<LightComponent>())
	{
		Renderer::GetInstance()->DeleteLight(go->GetComponent<LightComponent>());
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

	UIStatics::SetSelectedObject(nullptr);
}

const std::unordered_map<std::string, GameObject*>& SceneGraph::GetInstantiableObjects() const
{
	return InstantiableObjects;
}

bool SceneGraph::isObjectActive(const std::string& objName)
{
	for (auto* obj : gameObjects)
	{
		if (obj->name == objName)
		{
			return true;
		}
	}

	return false;
}

void SceneGraph::CheckCollisions()
{
	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		for (size_t j = i + 1; j < rigidBodies.size(); j++)
		{
			CollisionDetection::ColliderIntersection(rigidBodies[i]->GetCollider(), rigidBodies[j]->GetCollider());
		}
	}
}
