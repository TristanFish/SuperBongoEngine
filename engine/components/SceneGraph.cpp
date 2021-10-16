#include "SceneGraph.h"
#include "GameObject.h"
#include "core/Logger.h"
#include "core/3D/Physics3D.h"
#include "core/Globals.h"
#include "core/resources/SaveFile.h"
#include "Primitives/PlaneObject.h"
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

	Renderer::GetInstance()->DestroyRenderer();

	gameObjects.clear();

	rigidBodies.clear();
}

void SceneGraph::Init() 
{
	//osp = OctSpatialPartition(500);

	Renderer::GetInstance()->Init();


	for (auto obj : SaveManager::SaveableObjects)
	{

		if (obj.second->GetClone()->canBeInstantiated)
		{
			InstantiableObjects.emplace(obj.first, obj.second->GetClone());
		}
	}
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
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//Bind the gbuffer and clear it
	Renderer::GetInstance()->gBuffer.Bind();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


#ifdef _DEBUG
	for (auto* g : gameObjects)
	{
		g->DrawDebugGeometry();
	}
#endif // DEBUG

	Renderer::GetInstance()->Render();
}

void SceneGraph::HandleEvents(const SDL_Event& event)
{
	for (auto g : gameObjects)
	{
		g->HandleEvents(event);
	}
}

//Finds THE FIRST gameobject with the given name
GameObject& SceneGraph::FindGameObject(const char* name)
{
	for (auto* g : gameObjects)
	{
		if (g->name == name)
		{
			return *g;
		}
	}

	std::cerr << "No object named \"" << name << "\" was found. Messing things up so that you know something went wrong" << std::endl;

	for (auto* g : gameObjects)
	{
		g->transform.SetPos(MATH::Vec3(static_cast<float>(rand() % 100), static_cast<float>(rand() % 100), static_cast<float>(rand() % 100)));
		g->transform.SetScale(MATH::Vec3(static_cast<float>(rand() % 100), static_cast<float>(rand() % 100), static_cast<float>(rand() % 100)));
	}

	return *gameObjects[0];
}

//Adds a gameobject with a name and position
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

void SceneGraph::LoadGameObject(GameObject* go)
{
	go->Init();

	gameObjects.emplace_back(go);

	if (go->HasComponent<RigidBody3D>())
	{
		rigidBodies.emplace_back(go->GetComponent<RigidBody3D>());
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

	EngineLogger::Info(std::string(go->name) + " added to objectList", "ECS.cpp", __LINE__);

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

std::unordered_map<std::string, GameObject*> SceneGraph::GetInstantiableObjects()
{
	return InstantiableObjects;
}

bool SceneGraph::isObjectActive(std::string objName)
{
	for (auto obj : gameObjects)
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
			Physics3D::DetectCollision(*rigidBodies[i], *rigidBodies[j]);
		}
	}
}
