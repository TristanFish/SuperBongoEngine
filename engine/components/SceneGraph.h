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


	
	
	std::string prevLoadedObjName;

public:
	//OctSpatialPartition osp;

	//! SceneGraph Destructor
	/*! Destroys all of the gameobjects and clears all vectors used by this class*/
	~SceneGraph();

	//! SceneGraph Initializer
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

	void LoadGameObject(GameObject* go);

	std::unordered_map<std::string, GameObject*> GetInstantiableObjects();

	//! GetNumObject Getter
	/*!Returns the number of gameobjects in the scene*/
	 int GetNumObjects() const { return gameObjects.size(); }

	//! GetNumObject Getter
	/*!Returns the vector/list of gameobjects in the scene*/
	 const std::vector<GameObject*>& GetGameObjects() const { return gameObjects; }


	void LoadObject(SaveFile& file);



	//! CheckCollisions Function
	/*!Check's if any of the gameobjects are colliding*/
	void CheckCollisions();


};

#endif