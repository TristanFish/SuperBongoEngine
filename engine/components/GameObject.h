#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "core/Logger.h"
#include "Transform.h"
#include "Components.h"
#include "SDL_events.h"
#include <vector>


class RigidBody3D;

//! GameObject Class
/*!Things should be inheriting from gameobject, gameobjects are placed into a manager
Update, render and handleEvents need to be defined per class that inherits from it.
Check Player.h to see how to create gameobjects*/
class GameObject
{


protected:
	friend class SceneGraph;
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

#endif