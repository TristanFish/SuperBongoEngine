#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "core/Logger.h"
#include "core/3D/Physics/Collider3D.h"
#include "core/UUniqueID.h"

#include "Transform.h"
#include "Components.h"
#include "SDL_events.h"
#include <vector>


//! GameObject Class
/*!Things should be inheriting from gameObject, gameobjects are placed into a manager
Update, render and handleEvents need to be defined per class that inherits from it.
Check Player.h to see how to create gameobjects*/
class GameObject
{


protected:

	/*! Hold's the name of this gameObject*/
	std::string name;

	UUniqueID uuid;

	//! Active boolean
	/*! Controls if the gameObject is active or not*/
	bool active = true;

	GameObject* parent;
	std::vector<GameObject*> children;
	std::vector<Component*> componentList;



	friend class SceneGraph;
public:



	//!IsMenuActive boolean
	/*! Controls if the gameObject's properties panel is active*/
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

	//!Init Function
	/*!Initializes all components in the gameObject called whenever a gameobject is added to the sceneGraph*/
	virtual void Init();

	//!Begin Function
	/*!Meant to be overriden, is called after all objects are added to the scenegraph
	 * think of it as Unreal Engine BeginPlay() or Unity Start() */
	virtual void PostInit();
	
	//!Virtual Update Function
	/*!Updates the Gameobject position/rotation/translation*/
	virtual void Update(const float deltaTime);

	//!Virtual HandleEvents Function
	/*!Handles and Keyboard/Mouse events*/
	virtual void HandleEvents(const SDL_Event& event);

	//!Virtual DrawDebugGeometry Function
	/*!Draws the geometry of the object in wireframe*/
	virtual void DrawDebugGeometry() const {}

	virtual GameObject* NewClone() const = 0;

	//GetType function
	/*!Returns the type of class that the owning class is*/
	inline const char* GetType() const { return typeid(*this).name(); }

	//!isActive Getter
	/*!Returns if the gameObject is active or not*/
	bool isActive()const { return active; }

	//!isActive Setter
	/*!Sets the gameObject as active or not*/
	void SetActive(const bool a) { active = a; }

	std::string GetName() const { return name; }
	std::string& GetName() { return name; }

	uint64_t GetUUID() const { return uuid; }


	//!GetModelMatrix Getter
	/*!Returns the gameObject model matrix*/
	 MATH::Matrix4& GetModelMatrix() { return transform.GetModelMatrix(); }

	//!SetPos Setter
	/*!Sets the position of this a gameObject*/
	void SetPos(const MATH::Vec3& pos_) {  transform.SetPos(pos_); }

	//!SetScale Setter
	/*!Sets the scale of this a gameObject*/
	void SetScale(const MATH::Vec3& scale_) { transform.GetScale() = scale_; }

	//!SetRotation Setter
	/*!Sets the rotation of this a gameObject*/
	void SetRotation(const MATH::Vec3& rotation_) { transform.SetRot(rotation_); }

	/*!Sets the Name of this a gameObject*/
	void SetName(const std::string& name_) { name = name_; }


	void SetUUID(const uint64_t& uuid_) { uuid = UUniqueID(uuid_); }

	 GameObject* GetParent() const { return parent; }
	
	 GameObject* GetChild(int i) const { return children[i]; }

	 std::vector<GameObject*>& GetChildren() { return children; }

	 const std::vector<Component*>& GetComponents() const { return componentList; }

	 int GetChildCount() const { return children.size(); }
	
	bool operator == (const GameObject* v) const { return name == v->name; }

	//This functor is used for OnCollisionEnter functions for gameobjects
	virtual void OnCollisionEnter(Collider3D& otherCollider) {}
	//This functor is used for Attaching uniforms
	virtual void AttachUniforms() const {}

private:

	template <typename T>
	void CheckIfTemplateTypeInheritsFromComponent() const { static_assert(std::is_base_of<Component, T>::value, "Non Component type was used in a component based gameObject function"); }

public:

	//!Template HasComponent boolean
	/*!Checks if this gameObject has the specified component*/
	
	
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
	/*!Returns a component of type "T" if this gameObject has it*/
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
		EngineLogger::Warning("Component " + std::string(typeid(T).name()) + " not found in " + std::string(name), "ECS.h", __LINE__);
		return nullptr;
	}

	template <typename T>
	T* AddComponent()
	{
		CheckIfTemplateTypeInheritsFromComponent<T>();
		for(auto it = componentList.begin(); it != componentList.end(); ++it)
		{
			if(T* compType = dynamic_cast<T*>(*it))
			{
				EngineLogger::Warning("Component " + std::string((*it)->GetType()) + " already found in " + std::string(name), "ECS.h", __LINE__);
				return compType;
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
				delete *it;
				componentList.erase(it);
				return;
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