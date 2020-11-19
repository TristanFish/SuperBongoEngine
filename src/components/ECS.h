#ifndef ECS_HEADER
#define ECS_HEADER

#include <vector>
#include "math/Vector.h"
#include "components/Transform.h"
#include "sdl/SDL.h"
#include "core/3D/Renderer.h"

class GameObject;
class RigidBody3D;
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

	bool isMenuActive = false;

	// Object ID is used so we know what object is being spawned 
	// all objects that inherit from grass will have the same ID ext
	int objectID;

	Transform transform;

	GameObject();

	virtual ~GameObject();

	virtual void Update(const float deltaTime) = 0;
	virtual void Render() const = 0;
	virtual void HandleEvents(const SDL_Event& event) = 0;
	virtual void DrawDebugGeometry() {};

	inline bool isActive()const { return active; }
	inline void SetActive(bool a) { active = a; }

	inline MATH::Matrix4& GetModelMatrix() { return transform.GetModelMatrix(); }

	inline void SetPos(MATH::Vec3 pos_) {  transform.setPos(pos_); }
	inline void SetScale(MATH::Vec3 scale_) { transform.scale = scale_; }
	inline void SetRotation(MATH::Vec3 rotation_) { transform.rotation = rotation_; }
	inline void SetName(const char* name_) { name = name_; }
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
	std::vector<RigidBody3D*> rigidBodies;

	Renderer renderer;
public:

	~Manager();
	void Init();
	void Update(const float deltaTime);
	void Render() const;
	void HandleEvents(const SDL_Event& event);

	//Finds THE FIRST gameobject with the given name
	GameObject& FindGameObject(const char* name);
	//Adds a gameobject with a name and position
	GameObject& AddGameObject(GameObject* go, unsigned int objID);

	const int GetNumObjects() { return gameObjects.size(); }

	const std::vector<GameObject*> GetGameObjects() { return gameObjects; }

	void CheckCollisions();
};

#endif 