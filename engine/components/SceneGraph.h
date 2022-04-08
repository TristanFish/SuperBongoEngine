#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "rendering/Renderer.h"
#include "SDL_events.h"
#include <vector>
#include <unordered_map>


class RigidBody3D;
class GameObject;
class SaveFile;
class OctSpatialPartition;
//! SceneGraph Class
/*!There'll be one manager per scene it holds an array of gameobjects to update/render etc.
see how its used in scene1*/
class SceneGraph
{
private:

	//! Vector of GameObject pointers 
	/*! Holds all of the gameobjects in our scene*/
	std::vector<std::shared_ptr<GameObject>> gameObjects;

	//! Vector of RgidBody3D pointers 
	/*! Holds all of the rigidbodies in our scene*/
	std::vector<RigidBody3D*> rigidBodies;


	OctSpatialPartition* ScenePartition;

	//!
	/*! tracks whether the scenegraph is past the init stage */
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

	//! Update Physics Function
	/*! Updates all of the physics body's & collisions */
	void UpdatePhysics(const float deltaTime);

	//! Render Function
	/*! Renders all of the gameobjects using the renderer */
	void Render() const;
	void HandleEvents(const SDL_Event& event);

	void Clear();

	//! FindGameObject Function
	/*!Returns the first gameObject with a given name*/
	std::shared_ptr<GameObject> FindGameObject(const std::string& name);

	//! FindGameObject Function
	/*!Returns the first gameObject with a given name*/
	std::shared_ptr<GameObject> FindGameObject(const uint32_t& uuid);

	//! GameObjectNetworkUpdate Function
	/*!Finds and updates networkable game object based on data received from server*/
	void GameObjectNetworkUpdate(std::string& string);

	//! AddGameObject Function
	/*!Adds a gameObject with a pointer to a new gameObject and a Object ID*/
	const std::shared_ptr<GameObject> AddGameObject(std::shared_ptr<GameObject> go);

	//! GetNumObject Getter
	/*!Returns the number of gameobjects in the scene*/
	 int GetNumObjects() const { return gameObjects.size(); }

	//! GetNumObject Getter
	/*!Returns the vector/list of gameobjects in the scene*/
	const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const { return gameObjects; }
	const std::vector<RigidBody3D*>& GetRigidBodies() const { return rigidBodies; }

	OctSpatialPartition* GetScenePartition() const { return ScenePartition; }

	//! CheckCollisions Function
	/*!Check's if any of the gameobjects are colliding*/
	void CheckCollisions();

	void LoadGameObject(std::shared_ptr<GameObject> go);
	void DeleteGameObject(std::shared_ptr <GameObject> go);
};

#endif