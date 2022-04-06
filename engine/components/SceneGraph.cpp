#include "SceneGraph.h"
#include "GameObject.h"

#include "core/Logger.h"
#include "core/Globals.h"
#include "core/resources/SaveFile.h"
#include "core/resources/SaveManager.h"

#include "../game/gameObjects/Grass.h"
#include "../game/gameObjects/LightObject.h"

#include "core/resources/CollisionDetection.h"
#include <core/CoreEngine.h>

SceneGraph::~SceneGraph() 
{
	

	
	gameObjects.clear();

	rigidBodies.clear();
}

void SceneGraph::Init() 
{
	//ScenePartition = new OctSpatialPartition(1500);

	

	for(const auto& go : gameObjects)
	{
		go->Init();
	}
}

void SceneGraph::PostInit()
{
	for(const auto& go : gameObjects)
	{
		go->PostInit();
	}
	sceneIsPostInit = true;
}

void SceneGraph::Update(const float deltaTime)
{
	if(CoreEngine::GetInstance()->GetIsGameRunning())
	{
		for (const auto& g : gameObjects)
		{
			if (g->isActive())
			{
				g->Update(deltaTime);
			} else 
			{
				g->UpdateTransform();
			}
		}
	} else
	{
		for (const auto& g : gameObjects)
		{
			g->UpdateTransform();
		}
	}
}

void SceneGraph::UpdatePhysics(const float deltaTime)
{
	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		if (rigidBodies[i]->active)
		{
			rigidBodies[i]->Update(deltaTime);
		}

		for (size_t j = i + 1; j < rigidBodies.size(); j++)
		{
			CollisionDetection::ColliderIntersection(rigidBodies[i]->GetCollider(), rigidBodies[j]->GetCollider());
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
	for (const auto& g : gameObjects)
	{
		g->HandleEvents(event);
	}
}

//Finds THE FIRST gameObject with the given name
std::shared_ptr<GameObject> SceneGraph::FindGameObject(const std::string& name)
{
	for (const auto& g : gameObjects)
	{
		if (g->name == name)
		{
			return g;
		}
	}

	//EngineLogger::Error(std::string("No object named \"" + std::string(name) + "\" was found."), "SceneGraph.cpp", __LINE__);

	return nullptr;
}

std::shared_ptr<GameObject> SceneGraph::FindGameObject(const uint32_t& uuid)
{
	for (const auto& g : gameObjects)
	{
		if (g->GetUUID() == uuid)
		{
			return g;
		}
	}

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
const std::shared_ptr<GameObject> SceneGraph::AddGameObject(std::shared_ptr<GameObject> go)
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
	
	return go;
}

void SceneGraph::LoadGameObject(std::shared_ptr<GameObject> go)
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

	for (std::shared_ptr<GameObject> child : go->children)
	{
		AddGameObject(child);
	}
}

void SceneGraph::DeleteGameObject(std::shared_ptr<GameObject> go)
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

	for (std::vector<std::shared_ptr<GameObject>>::iterator iter = gameObjects.begin(); iter != gameObjects.end(); iter++)
	{
		if (*iter == go)
		{
			go.reset();
			gameObjects.erase(iter);
			break;
		}
	}
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
