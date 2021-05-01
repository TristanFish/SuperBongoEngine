#include "ECS.h"
#include "3D/RigidBody3D.h"
#include "core/3D/Physics3D.h"
#include "3D/MeshRenderer.h"
#include "3D/LightComponent.h"
#include "core/Logger.h"

GameObject::GameObject(): name("Default")
{
}

GameObject::~GameObject()
{
	name = nullptr;

	for(Component* comp : componentList)
	{
		if(comp)
		{
			delete comp;
			comp = nullptr;
		}
	}
	componentList.clear();
}

void GameObject::Init()
{
	for (Component* comp : componentList)
	{
		comp->Init(this);
		if(RigidBody3D* rbComp = dynamic_cast<RigidBody3D*>(comp))
		{
			rbComp->AddCollisionFunction(std::bind(&GameObject::OnCollisionEnter, this, std::placeholders::_1));
		}
		else if(MeshRenderer* mComp = dynamic_cast<MeshRenderer*>(comp))
		{
			mComp->AddUniformFunction(std::bind(&GameObject::AttachUniforms, this));
		}
	}
}

void GameObject::Update(const float deltaTime)
{
	transform.Update(deltaTime);
	for(Component* comp : componentList)
	{
		if(comp->active)
		{
			comp->Update(deltaTime);
		}
	}
}

void GameObject::HandleEvents(const SDL_Event& event)
{
	for(Component* comp : componentList)
	{
		if(comp->active)
		{
			comp->HandleEvents(event);
		}
	}
}


Manager::~Manager()
{
	for (GameObject* g : gameObjects)
	{
		if (g)
		{
			delete g;
			g = nullptr;
		}
	}

	renderer.DestroyRenderer();

	gameObjects.clear();

	rigidBodies.clear();
}

void Manager::Init()
{
	osp = OctSpatialPartition(500);
	
	for(GameObject* go : gameObjects)
	{
		go->Init();
	}
	renderer.Init();
}

void Manager::Update(const float deltaTime)
{
	for (auto* g : gameObjects)
	{
		if (g->isActive())
		{
			g->Update(deltaTime);
		}
	}
}
void Manager::Render() const
{
	//Clear the default framebuffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//Bind the gbuffer and clear it
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderer.gBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


#ifdef _DEBUG
	for (auto* g : gameObjects)
	{
		g->DrawDebugGeometry();
	}
#endif // DEBUG

	renderer.Render();
}

void Manager::HandleEvents(const SDL_Event& event)
{
	for (auto g : gameObjects)
	{
		g->HandleEvents(event);
	}
}

//Finds THE FIRST gameobject with the given name
GameObject& Manager::FindGameObject(const char* name)
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
GameObject& Manager::AddGameObject(GameObject* go)
{
	gameObjects.emplace_back(go);
	if (go->HasComponent<RigidBody3D>())
	{
		rigidBodies.emplace_back(go->GetComponent<RigidBody3D>());
	}
	if (go->HasComponent<MeshRenderer>())
	{
		MeshRenderer* mr = go->GetComponent<MeshRenderer>();
		renderer.AddMeshRenderer(mr);
		osp.AddObject(mr);
	}
	if (go->HasComponent<LightComponent>())
	{
		renderer.AddLight(go->GetComponent<LightComponent>());
	}

	EngineLogger::Info(std::string(go->name) + " added to objectList", "ECS.cpp", __LINE__);

	for (GameObject* child : go->children)
	{
		AddGameObject(child);
	}
	
	return *go;
}

void Manager::CheckCollisions()
{
	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		for (size_t j = i + 1; j < rigidBodies.size(); j++)
		{
			Physics3D::DetectCollision(*rigidBodies[i], *rigidBodies[j]);
		}
	}
}
