#include "ECS.h"
#include "3D/RigidBody3D.h"
#include "core/3D/Physics3D.h"
#include "3D/MeshRenderer.h"
#include "3D/LightComponent.h"


GameObject::GameObject(): name("Default"), transform()
{
}


GameObject::~GameObject()
{
	name = nullptr;
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
	renderer.Init();
}

void Manager::Update(const float deltaTime)
{
	for (auto g : gameObjects)
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
	for (auto g : gameObjects)
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
	for (auto g : gameObjects)
	{
		if (g->name == name)
		{
			return *g;
		}
	}

	std::cerr << "No object named \"" << name << "\" was found. Messing things up so that you know something went wrong" << std::endl;

	for (auto g : gameObjects)
	{
		g->transform.GetPosition() = MATH::Vec3(static_cast<float>(rand() % 100), static_cast<float>(rand() % 100), static_cast<float>(rand() % 100));
		g->transform.GetScale() = MATH::Vec3(static_cast<float>(rand() % 100), static_cast<float>(rand() % 100), static_cast<float>(rand() % 100));
	}

	return *gameObjects[0];
}

//Adds a gameobject with a name and position
GameObject& Manager::AddGameObject(GameObject* go, unsigned int objID)
{
	go->objectID = objID;
	gameObjects.emplace_back(go);
	if (go->hasComponent<RigidBody3D>())
	{
		rigidBodies.emplace_back(&go->getComponent<RigidBody3D>());
	}
	if (go->hasComponent<MeshRenderer>())
	{
		renderer.AddMeshRenderer(&go->getComponent<MeshRenderer>());
	}
	if (go->hasComponent<LightComponent>())
	{
		renderer.AddLight(&go->getComponent<LightComponent>());
	}

	std::cout << go->name << " added to objectList" << std::endl;
	return *go;
}

void Manager::CheckCollisions()
{
	//std::cout << rigidBodies.size() << std::endl;
	for (size_t i = 0; i < rigidBodies.size(); i++)
	{
		for (size_t j = i + 1; j < rigidBodies.size(); j++)
		{
			Physics3D::DetectCollision(*rigidBodies[i], *rigidBodies[j]);
		}
	}
}
