#ifndef ECS_HEADER
#define ECS_HEADER

#include <vector>
#include "math/Vector.h"
#include "components/Transform.h"
#include "sdl/SDL.h"
#include "core/3D/Renderer.h"
#include "core/Logger.h"

class GameObject;
class RigidBody3D;

//! Component Class
/*!Component is an interface, things should be inheriting from it
Components are added to a gameobject through inheritance
All component functions should be called through the gameobject class that inherits from them
Check Player.h to see how to add components*/
class Component
{
public:

	//!GameObject Pointer
	/*!Enables the components to know what gameobject they are attached too*/
	GameObject* gameobject;

	//!Virtual Init Function
	/*!Initializes all of the needed variables*/
	virtual void Init(GameObject *g) = 0;

	//!Virtual Update Function
	/*!Updates all of the components systems*/
	virtual void Update(const float deltaTime) = 0;

	//!Virtual Render Function
	/*!Renders anything needed for the component*/
	virtual void Render() const = 0;

	//!Virtual HandleEvents Function
	/*!Handles any events needed for the component*/
	virtual void HandleEvents(const SDL_Event& event) = 0;

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

	//! Active boolean
	/*! Control's if the gameobject is active or not*/
	bool active = true;

public:

	//!Name Char*
	/*! Hold's the name of this gameobject*/
	const char* name;

	//!IsMenuActive boolean
	/*! Controls if it's properties panel is active*/
	bool isMenuActive = false;

	//!Object ID Integer
	/*! Object ID is used so we know what object is being spawned 
	 all objects that inherit from grass will have the same ID ext*/
	int objectID;

	//!Transform
	/*! Control's all of the gameobjects positions/rotations/translations*/
	Transform transform;

	//!Gameobject Constructor
	/*! Empty Function*/
	GameObject();

	//!Virtual Gameobject Destructor
	/*!Deletes any pointers/clears any vectors*/
	virtual ~GameObject();

	//!Virtual Update Function
	/*!Updates the Gameobject position/rotation/translation*/
	virtual void Update(const float deltaTime) = 0;

	//!Virtual Render Function
	/*!Renders the Gameobject on screen*/
	virtual void Render() const = 0;

	//!Virtual HandleEvents Function
	/*!Handles and Keyboard/Mouse events*/
	virtual void HandleEvents(const SDL_Event& event) = 0;

	//!Virtual DrawDebugGeometry Function
	/*!Draws the geometry of the object in wireframe*/
	virtual void DrawDebugGeometry() {};

	//!inline isActive Getter
	/*!Returns if the gameobject is active or not*/
	inline bool isActive()const { return active; }

	//!inline isActive Setter
	/*!Sets the gameobject as active or not*/
	inline void SetActive(bool a) { active = a; }

	//!inline GetModelMatrix Getter
	/*!Returns the gameobject model matrix*/
	inline MATH::Matrix4& GetModelMatrix() { return transform.GetModelMatrix(); }

	//!inline SetPos Setter
	/*!Set's the position of this a gameobject*/
	inline void SetPos(MATH::Vec3 pos_) {  transform.setPos(pos_); }

	//!inline SetScale Setter
	/*!Set's the scale of this a gameobject*/
	inline void SetScale(MATH::Vec3 scale_) { transform.scale = scale_; }

	//!inline SetRotation Setter
	/*!Set's the rotation of this a gameobject*/
	inline void SetRotation(MATH::Vec3 rotation_) { transform.rotation = rotation_; }

	//!inline SetName Setter
	/*!Set's the Name of this a gameobject*/
	inline void SetName(const char* name_) { name = name_; }

	//!Template hasComponent boolean
	/*!Checks if this gameobject has the specified component*/
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

	//!Template getComponent function
	/*!Returns a component of type "T" if this gameobject has it*/
	template <typename T>
	T& getComponent()
	{
		return dynamic_cast<T&>(*this);
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
	GameObject& AddGameObject(GameObject* go, unsigned int objID);

	//! GetNumObject Getter
	/*!Returns the number of gameobjects in the scene*/
	const int GetNumObjects() { return gameObjects.size(); }

	//! GetNumObject Getter
	/*!Returns the vector/list of gameobjects in the scene*/
	const std::vector<GameObject*> GetGameObjects() { return gameObjects; }

	//! CheckCollisions Function
	/*!Check's if any of the gameobjects are colliding*/
	void CheckCollisions();
};

#endif 