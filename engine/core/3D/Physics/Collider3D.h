#ifndef COLLIDER3D_H
#define COLLIDER3D_H

//Not quite a component but similar enough to be in the same folder path
//Colliders define how rigidbodies will collide


#include <functional>

#include "math/Vector.h"
#include "graphics/Vertex.h"

class RigidBody3D;

enum class ColliderType : char
{
	Sphere = 0b0001,
	Plane = 0b0010,
	OBB = 0b0100,
	OCTNODE = 0b1000
};

inline constexpr ColliderType operator | (ColliderType x, ColliderType y)
{
	return static_cast<ColliderType>(static_cast<char>(x) | static_cast<char>(y));
}

class Collider3D
{
protected:


	bool B_IsTrigger;

	bool B_IsMoveable;

	MATH::Vec3 V_Size;

	ColliderType Type;

	class RigidBody3D* RB_Attached;

	std::function<void(Collider3D&)> collisionEnterCallback;
	
	//! Model_Verticies vector
	/*! Stores all the verticies before being transfered to world space*/
	std::vector<Vertex>* Model_Verticies;

public:


	Collider3D();
	virtual ~Collider3D();

	void OnCollisionEnter(Collider3D& otherCollider);

	void AddCollisionFunction(std::function<void(Collider3D&)> func);



	inline bool IsType(ColliderType type, const std::vector<Collider3D*>& colliders = std::vector<Collider3D*>())
	{
		bool SameType = false;

		if (Type != type)
		{
			return SameType;
		}
		SameType = true;

		for (const auto& collider : colliders)
		{
			if (collider->Type != type)
			{
				SameType = false;
				break;
			}
		}
		return SameType;
	}

	inline bool operator==(const Collider3D& collider) { return Type == collider.Type; }


	inline void SetTrigger(bool NewIsTrigger) { B_IsTrigger = NewIsTrigger; }
	inline void SetMoveable(bool NewIsMoveable) { B_IsMoveable = NewIsMoveable; }
	virtual inline void SetSize(const MATH::Vec3& NewSize) { V_Size = NewSize; }
	inline void SetColliderType(ColliderType NewType) { Type = NewType; }
	inline void SetModelVerticies(std::vector<Vertex>& New_Verticies) { Model_Verticies = &New_Verticies; }


	inline bool IsTrigger() const { return B_IsTrigger; }
	inline bool IsMoveable() const { return B_IsMoveable; }
	inline MATH::Vec3 GetSize() const { return V_Size; }
	inline ColliderType GetColliderType() const { return Type; }
	inline class RigidBody3D* GetRBAttached() { return RB_Attached; }

	//!  GetModelVerticies Getter
	/*!  Returns the Model_Verticies vector by const ref*/
	inline const std::vector<Vertex>* GetModelVerticies() { return Model_Verticies; }

	friend class RigidBody3D;
};


#endif