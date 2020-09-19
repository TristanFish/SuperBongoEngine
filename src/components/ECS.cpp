#include "ECS.h"
#include "components/2D/RigidBodyComponent.h"

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

	gameObjects.clear();

	rigidBodies.clear();
}

void Manager::Init()
{
	for (GameObject* g : gameObjects)
	{
		if (g->hasComponent<RigidBodyComponent>())
		{
			rigidBodies.emplace_back(&g->getComponent<RigidBodyComponent>());
		}
	}
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
	for (auto g : gameObjects)
	{
		if (g->isActive())
		{
			g->Render();
		}
	}
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
GameObject& Manager::AddGameObject(GameObject* go)
{
	gameObjects.emplace_back(go);
	std::cout << go->name << " added to objectList" << std::endl;
	return *go;
}