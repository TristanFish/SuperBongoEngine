#include "ECS.h"
#include "components/RigidBodyComponent.h"

GameObject::GameObject(const char* n, const MATH::Vec3& pos): name(n), transform(new Tranform(pos))
{
}

GameObject::~GameObject()
{
	delete name;
	name = nullptr;
	
	delete transform;
	transform = nullptr;

}

void GameObject::Update(const float deltaTime)
{
	transform->Update(deltaTime);
	
}

void GameObject::Render() const
{
	
}

template <typename T>
bool GameObject::hasComponent(T) const
{
	return (dynamic_cast<T>(this));
}




template <typename T>
T& GameObject::getComponent(T) const
{
	return dynamic_cast<T&>(this);
}


Manager::~Manager()
{
	for (GameObject* g : gameObjects)
	{
		delete g;
		g = nullptr;
	}

	for (RigidBodyComponent* r : rigidBodies)
	{
		delete r;
		r = nullptr;
	}
}

void Manager::Init()
{
	for (GameObject* g : gameObjects)
	{
		if (RigidBodyComponent* r = dynamic_cast<RigidBodyComponent*>(g))
		{
			rigidBodies.emplace_back(r);
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
		g->transform->GetPosition() = MATH::Vec3(rand() % 100, rand() % 100, rand() % 100);
		g->transform->GetScale() = MATH::Vec3(rand() % 100, rand() % 100, rand() % 100);
	}

	return *gameObjects[0];
}

//Adds a gameobject with a name and position
GameObject& Manager::addGameObject(GameObject* go)
{
	GameObject* g = go;

	gameObjects.emplace_back(g);
	return *g;
}