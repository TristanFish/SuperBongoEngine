#ifndef ECS_HEADER
#define ECS_HEADER

#include <vector>
//#include <memory>
#include <bitset>
#include <array>
#include "math/Vector.h"
#include "components/Transform.h"
#include "sdl/SDL.h"


class Component;
class GameObject;
class RigidBodyComponent;
using ComponentID = std::size_t;


//Component is an interface, things should be inheriting from it, components are placed into a gameobject
class Component
{
public:
	GameObject* gameObject;
	
	virtual void Init() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event& event) = 0;


	virtual ~Component() { gameObject = nullptr; }
};

//This should be inheriting from gameobject, gameobjects are placed into a manager
//GameObject update, render and handleEvents need to be defined per class that inherits from it.
class GameObject
{
protected:
	bool active = true;

public:
	const char* name;
	Tranform* transform;

	GameObject(const char* n, const MATH::Vec3& pos);

	virtual ~GameObject();

	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event& event) = 0;

	inline bool isActive()const { return active; }
	inline void SetActive(bool a) { active = a; }

	template <typename T>
	bool hasComponent(T) const;

	Component& addComponent(Component& c);

	template <typename T>
	T& getComponent(T) const;
};

//There'll be one manager per scene it holds an array of gameobjects to update/render etc.
class Manager
{
private:
	std::vector<GameObject*> gameObjects;
	std::vector<RigidBodyComponent*> rigidBodies;
public:

	~Manager();
	void Init();
	void Update(const float deltaTime);
	void Render() const;
	void HandleEvents(const SDL_Event& event);

	//Finds THE FIRST gameobject with the given name
	GameObject& FindGameObject(const char* name);
	//Adds a gameobject with a name and position
	GameObject& addGameObject(GameObject* go);
};

#endif 