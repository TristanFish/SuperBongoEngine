#ifndef ECS_HEADER
#define ECS_HEADER

#include <vector>
#include <type_traits>
#include <memory>

#include "math/Vector.h"
#include "components/Transform.h"
#include "sdl/SDL.h"
#include "core/3D/Renderer.h"
#include "core/Logger.h"
#include "core/OctSpatialPartition.h"


class GameObject;
class RigidBody3D;

//! Component Class
/*!Component is an interface, 
All component functions should be called through the gameobject class that inherits from them
Check Player.h to see how to add components*/
class Component
{
public:

	//!GameObject Pointer
	/*!Enables the components to know what gameobject they are attached too*/
	GameObject* gameobject;

	bool active = true;
	//!Virtual Init Function
	/*!Initializes all of the needed variables*/
	virtual void Init(GameObject *g) = 0;

	//!Virtual Update Function
	/*!Updates all of the components systems*/
	virtual void Update(const float deltaTime) = 0;

	//!Virtual Render Function
	/*!Renders anything needed for the component*/
	virtual void Render() const {}

	//!Virtual HandleEvents Function
	/*!Handles any events needed for the component*/
	virtual void HandleEvents(const SDL_Event& event) = 0;

	virtual const char* GetType() { return typeid(*this).name(); }

	//!Virtual Destructor 
	/*!Destroys any of the pointers/vectors needed*/
	virtual ~Component() { gameobject = nullptr; }
};

//! GameObject Class
/*!Things should be inheriting from gameobject, gameobjects are placed into a manager
Update, render and handleEvents need to be defined per class that inherits from it.
Check Player.h to see how to create gameobjects*/
class GameObject
{


protected:
	friend class Manager;
	//! Active boolean
	/*! Control's if the gameobject is active or not*/
	bool active = true;

	GameObject* parent;
	std::vector<GameObject*> children;
	std::vector<Component*> componentList;

	// Enum Class Type
	/*!Holds enumerations to all the classes that can be instantiated in our engine*/
	enum class Type { None, Box, Plane, Sphere, Light, Grass };


	Type typeID = Type::None;

public:

	

	//!Name Char*
	/*! Hold's the name of this gameobject*/
	const char* name;

	//!IsMenuActive boolean
	/*! Controls if it's properties panel is active*/
	bool isMenuActive = false;

	//!Transform
	/*! Control's all of the gameobjects positions/rotations/translations*/
	Transform transform;

	//!Gameobject Constructor
	/*! Empty Function*/
	GameObject();

	//!Virtual Gameobject Destructor
	/*!Deletes any pointers/clears any vectors*/
	virtual ~GameObject();

	virtual void Init();
	
	//!Virtual Update Function
	/*!Updates the Gameobject position/rotation/translation*/
	virtual void Update(const float deltaTime);

	//GetType function
	/*!Returns the type of class that the owning class is*/
	inline Type getType() const { return typeID; }


	inline void SetType(Type typeID_) { typeID = typeID_; }


	//!Virtual HandleEvents Function
	/*!Handles and Keyboard/Mouse events*/
	virtual void HandleEvents(const SDL_Event& event);

	//!Virtual DrawDebugGeometry Function
	/*!Draws the geometry of the object in wireframe*/
	virtual void DrawDebugGeometry() const {}

	virtual const char* GetType() { return typeid(*this).name(); }
	//!isActive Getter
	/*!Returns if the gameobject is active or not*/
	bool isActive()const { return active; }

	//!isActive Setter
	/*!Sets the gameobject as active or not*/
	void SetActive(const bool a) { active = a; }

	std::string GetName() const { return std::string(name); }

	//!GetModelMatrix Getter
	/*!Returns the gameobject model matrix*/
	const MATH::Matrix4& GetModelMatrix() const { return transform.GetModelMatrix(); }

	//!SetPos Setter
	/*!Set's the position of this a gameobject*/
	void SetPos(const MATH::Vec3& pos_) {  transform.SetPos(pos_); }

	//!SetScale Setter
	/*!Set's the scale of this a gameobject*/
	void SetScale(const MATH::Vec3& scale_) { transform.scale = scale_; }

	//!SetRotation Setter
	/*!Set's the rotation of this a gameobject*/
	void SetRotation(const MATH::Vec3& rotation_) { transform.rotation = rotation_; }

	//!SetName Setter
	/*!Set's the Name of this a gameobject*/
	void SetName(const char* name_) { name = name_; }


	//This functor is used for OnCollisionEnter functions for gameobjects
	virtual void OnCollisionEnter(RigidBody3D& otherBody) {}
	//This functor is used for Attaching uniforms
	virtual void AttachUniforms() const {}

private:

	template <typename T>
	void CheckIfTemplateTypeInheritsFromComponent() const { static_assert(std::is_base_of<Component, T>::value, "Non Component type was used in a component based gameobject function"); }

public:

	//!Template HasComponent boolean
	/*!Checks if this gameobject has the specified component*/
	template <typename T>
	bool HasComponent()
	{
		CheckIfTemplateTypeInheritsFromComponent<T>();

		for(auto it = componentList.begin(); it != componentList.end(); ++it)
		{
			if(dynamic_cast<T*>(*it))
			{
				return true;
			}
		}
		return false;
	}

	//!Template GetComponent function
	/*!Returns a component of type "T" if this gameobject has it*/
	template <typename T>
	T* GetComponent()
	{
		CheckIfTemplateTypeInheritsFromComponent<T>();
		
		for(auto it = componentList.begin(); it != componentList.end(); ++it)
		{
			if(T* comp = dynamic_cast<T*>(*it))
			{
				return comp;
			}
		}
		EngineLogger::Error("Component " + std::string(typeid(T).name()) + " not found in " + std::string(name), "ECS.h", __LINE__);
		return nullptr;
	}

	template <typename T>
	T* AddComponent()
	{
		CheckIfTemplateTypeInheritsFromComponent<T>();
		for(auto it = componentList.begin(); it != componentList.end(); ++it)
		{
			if(dynamic_cast<T*>(*it))
			{
				EngineLogger::Warning("Component " + std::string((*it)->GetType()) + " already found in " + std::string(name), "ECS.h", __LINE__);
				return nullptr;
			} 
		}
		componentList.emplace_back(new T());
		return GetComponent<T>();
	}

	template <typename T>
	void RemoveComponent()
	{
		CheckIfTemplateTypeInheritsFromComponent<T>();
		for(auto it = componentList.begin(); it != componentList.end(); ++it)
		{
			if(dynamic_cast<T*>(*it))
			{
				componentList.erase(it);
			}
		}
		EngineLogger::Info("No component of type " + std::string(typeid(T).name()) + " found in " + std::string(name), "ECS.h", __LINE__);
	}

	template <typename T>
	T* AddChild(GameObject* go)
	{
		children.emplace_back(go);
		go->parent = this;
		return dynamic_cast<T*>(go);
	}

};

//! Manager Class
/*!There'll be one manager per scene it holds an array of gameobjects to update/render etc.
see how its used in scene1*/
class Manager
{
private:

	//! Vector of GameObject pointers 
	/*! Holds all of the gameobjects in our scene*/
	std::vector<GameObject*> gameObjects;

	//! Vector of RgidBody3D pointers 
	/*! Holds all of the rigidbodies in our scene*/
	std::vector<RigidBody3D*> rigidBodies;


	//! Renderer 
	/*! Handles the rendering of all the gameobjects*/
	Renderer renderer;

public:
	OctSpatialPartition osp;

	//! Manager Destructor
	/*! Destroys all of the gameobjects and clears all vectors used by this class*/
	~Manager();

	//! Manager Initializer
	/*! Initialized the renderer */
	void Init();

	//! Update Function
	/*! Updates all of the gameobjects in the "gameObjects" vector */
	void Update(const float deltaTime);

	//! Render Function
	/*! Renders all of the gameobjects using the renderer */
	void Render() const;
	void HandleEvents(const SDL_Event& event);

	//! FindGameObject Function
	/*!Returns the first gameobject with a given name*/
	GameObject& FindGameObject(const char* name);

	//! AddGameObject Function
	/*!Adds a gameobject with a pointer to a new gameobject and a Object ID*/
	GameObject& AddGameObject(GameObject* go);

	//! GetNumObject Getter
	/*!Returns the number of gameobjects in the scene*/
	int GetNumObjects() const { return gameObjects.size(); }

	//! GetNumObject Getter
	/*!Returns the vector/list of gameobjects in the scene*/
	const std::vector<GameObject*>& GetGameObjects() const { return gameObjects; }

	//! CheckCollisions Function
	/*!Check's if any of the gameobjects are colliding*/
	void CheckCollisions();
};

#endif 