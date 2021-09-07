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
	/*! Controls if the gameobject is active or not*/
	bool active = true;

	GameObject* parent;
	std::vector<GameObject*> children;
	std::vector<Component*> componentList;




public:

	/*! Hold's the name of this gameobject*/
	std::string name;

	//!IsMenuActive boolean
	/*! Controls if the gameobject's properties panel is active*/
	bool isObjectSelected = false;


	//!canBeInstantiated boolean
	/*! Control's if the object can be spawned and will show up in the spawn able objects GUI list*/
	bool canBeInstantiated = false;

	//!Transform
	/*! Controls all of the gameobjects positions/rotations/translations*/
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

	//!Virtual HandleEvents Function
	/*!Handles and Keyboard/Mouse events*/
	virtual void HandleEvents(const SDL_Event& event);

	//!Virtual DrawDebugGeometry Function
	/*!Draws the geometry of the object in wireframe*/
	virtual void DrawDebugGeometry() const {}

	virtual GameObject* GetClone() const = 0;

	//GetType function
	/*!Returns the type of class that the owning class is*/
	inline const char* GetType() const { return typeid(*this).name(); }

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
	/*!Sets the position of this a gameobject*/
	void SetPos(const MATH::Vec3& pos_) {  transform.SetPos(pos_); }

	//!SetScale Setter
	/*!Sets the scale of this a gameobject*/
	void SetScale(const MATH::Vec3& scale_) { transform.scale = scale_; }

	//!SetRotation Setter
	/*!Sets the rotation of this a gameobject*/
	void SetRotation(const MATH::Vec3& rotation_) { transform.rotation = rotation_; }

	/*!Sets the Name of this a gameobject*/
	void SetName(std::string name_) { name = name_; }


	inline GameObject* GetParent() const { return parent; }
	
	inline GameObject* GetChild(int i) const { return children[i]; }

	inline std::vector<GameObject*>& GetChildren() { return children; }

	inline std::vector<Component*> GetComponents() const { return componentList; }

	inline int GetChildCount() const { return children.size(); }
	


	inline bool operator == (const GameObject* v) { return name == v->name; }

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

	GameObject* AddChild(GameObject* go)
	{
		children.emplace_back(go);
		go->parent = this;
		go->transform.SetParent(&this->transform);
		return go;
	}

	void RemoveChild(GameObject* go)
	{
		std::vector<GameObject*>::iterator iter = children.begin();

		while (iter != children.end())
		{
			if ((*iter) == go)
			{
				children.erase(iter);
				go->parent = nullptr;
				go->transform.SetParent(nullptr);
				break;
			}
			iter++;
		}
	}



	template <typename T>
	T* AddChild(GameObject* go)
	{
		children.emplace_back(go);
		go->parent = this;
		go->transform.SetParent(&this->transform);
		return dynamic_cast<T*>(go);
	}

};

#endif