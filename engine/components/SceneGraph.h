#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "core/3D/OctSpatialPartition.h"
#include "rendering/Renderer.h"
#include "SDL_events.h"
#include <vector>
#include <unordered_map>


class RigidBody3D;
class GameObject;
class SaveFile;
//! SceneGraph Class
/*!There'll be one manager per scene it holds an array of gameobjects to update/render etc.
see how its used in scene1*/
class SceneGraph
{
private:

	//! Vector of GameObject pointers 
	/*! Holds all of the gameobjects in our scene*/
	std::vector<GameObject*> gameObjects;

	//! Vector of RgidBody3D pointers 
	/*! Holds all of the rigidbodies in our scene*/
	std::vector<RigidBody3D*> rigidBodies;

	std::unordered_map<std::string, GameObject*>  InstantiableObjects;

	//!
	/*! tracks whether the scenegraph is past the */
	bool sceneIsPostInit = false;
	
	

public:
	//OctSpatialPartition osp;

	//! SceneGraph Destructor
	/*! Destroys all of the gameobjects and clears all vectors used by this class*/
	~SceneGraph();

	//! SceneGraph Initializer
	/*! Initialized the renderer */
	void Init();

	//! PostInit Function
	/*! Called after all objects have been added to the scenegraph (after the scene's OnCreate function)*/
	void PostInit();
	
	//! Update Function
	/*! Updates all of the gameobjects in the "gameObjects" vector */
	void Update(const float deltaTime);

	//! Render Function
	/*! Renders all of the gameobjects using the renderer */
	void Render() const;
	void HandleEvents(const SDL_Event& event);

	//! FindGameObject Function
	/*!Returns the first gameObject with a given name*/
	GameObject* FindGameObject(const char* name);

	//! AddGameObject Function
	/*!Adds a gameObject with a pointer to a new gameObject and a Object ID*/
	GameObject& AddGameObject(GameObject* go);

	void AddRenderingComponents();

	const std::unordered_map<std::string, GameObject*>& GetInstantiableObjects() const;

	//! GetNumObject Getter
	/*!Returns the number of gameobjects in the scene*/
	 int GetNumObjects() const { return gameObjects.size(); }

	 //! isObjectActive Getter
	/*!Return's if there is already an object with a given name*/
	 bool isObjectActive(std::string objName);

	//! GetNumObject Getter
	/*!Returns the vector/list of gameobjects in the scene*/
	 const std::vector<GameObject*>& GetGameObjects() const { return gameObjects; }

	//! CheckCollisions Function
	/*!Check's if any of the gameobjects are colliding*/
	void CheckCollisions();

	void LoadGameObject(GameObject* go);

	void DeleteGameObject(GameObject* go);
};

#endif