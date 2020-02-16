#ifndef ECS_HEADER
#define ECS_HEADER

#include <vector>
#include "math/Vector.h"
#include "components/Transform.h"
#include "sdl/SDL.h"


class Component;
class GameObject;
class RigidBodyComponent;


//Component is an interface, things should be inheriting from it
//Components are added to a gameobject through inheritance
//All component functions should be called through the gameobject class that inherits from them
//Check Player.h to see how to add components
class Component
{
public:
	GameObject* gameobject;

	//Use Init to set the gameobject pointer to "this"
	virtual void Init(GameObject *g) = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event& event) = 0;


	virtual ~Component() { gameobject = nullptr; }
};

//Things should be inheriting from gameobject, gameobjects are placed into a manager
//Update, render and handleEvents need to be defined per class that inherits from it.
//Check Player.h to see how to create gameobjects
class GameObject
{
protected:
	bool active = true;

public:
	const char* name;
	Transform transform;

	GameObject();

	virtual ~GameObject();

	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event& event) = 0;

	inline bool isActive()const { return active; }
	inline void SetActive(bool a) { active = a; }

	template <typename T>
	bool hasComponent()
	{
		if (dynamic_cast<T*>(this))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename T>
	T& getComponent()
	{
		return dynamic_cast<T&>(*this);
	}
};

//There'll be one manager per scene it holds an array of gameobjects to update/render etc.
//see how its used in scene1
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