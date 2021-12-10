#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <functional>
#include "components/ECS.h"
#include "math/Vector.h"

#include "core/3D/Physics/Collider3D.h"


//! RigidBody3D Class
/*! Is a component and handles almost all of the needed math for the physics of our engine */
class RigidBody3D : public Component	{
private:


	float mass;

	//! Vector3 Pointer Position
	/*! Is the position of our object in world space */
	MATH::Vec3* pos;
	MATH::Vec3 vel;
	MATH::Vec3 accel;

	//! Float Linear Drag
	/*! Is the linear drag of our object  */
	float linearDrag;

	//! Float Rotational Inertia 
	/*! Is the rotational inertia of our object  */
	float rotInertia;
	MATH::Vec3 angularVel;
	MATH::Vec3 angularAcc;


	//! Float Angular Drag 
	/*! Is the angular drag draw of our object  */
	float angularDrag;


	//! Collider3D Collider
	/*! Every RigidBody3D has a collider that allows it to collide with objects */
	class Collider3D* collider;


	//! Friend class Physics3D
	/*! Allows us to access all the variables from Physics3D */
	friend class Physics3D;

	
public:
	//! RigidBody3D Constructor
	RigidBody3D();

	//! RigidBody3D Destructor
	~RigidBody3D();
	
	// Inherited via Component
	void Init(GameObject *g) override;
	void Update(const float deltaTime) override;
	void HandleEvents(const SDL_Event& event) override;

	void ImGuiRender() override;

	void ApplyImpulseForce(const MATH::Vec3& force);
	void ApplyConstantForce(const MATH::Vec3& force);
	void SetConstantForce(const MATH::Vec3& force);

	void ApplyImpulseTorque(const MATH::Vec3& torque);
	void ApplyConstantTorque(const MATH::Vec3& torque);


	void ConstructCollider(ColliderType Collider_Type);
	void SetColliderSize(MATH::Vec3 s);
	void SetColliderType(ColliderType newType);
	ColliderType GetColliderType();

	bool isMoveable() const;
	void setMoveable(bool b);

	
	//Getters and setters
#pragma region getters/setters

	 Collider3D* GetCollider(); 

	inline MATH::Vec3 GetPosition() const { return *pos; }
	inline void SetPosition(const MATH::Vec3& p) const { *pos = p; }

	inline MATH::Vec3 GetVelocity() const  { return vel; }

	inline void SetVelocity(const MATH::Vec3& velocity) { vel = velocity; }
	inline MATH::Vec3 GetAccel() const { return accel; }


	inline void SetAccel(const MATH::Vec3& acc) { accel = acc; }
	inline float GetLinDrag() const { return linearDrag; }
	inline void SetLinDrag(const float drag) { linearDrag = drag; }
	inline void SetMass(const float m) { mass = m; }

	inline MATH::Vec3 GetAngVelocity() const { return angularVel; }
	inline MATH::Vec3& GetAngVelocity()  { return angularVel; }

	inline void SetAngVelocity(const MATH::Vec3& aVel) { angularVel = aVel; }
	inline MATH::Vec3 GetAngAccel() const { return angularAcc; }

	inline void SetAngAccel(const MATH::Vec3& acc) { angularAcc = acc; }
	inline float GetAngDrag() const { return angularDrag; }
	inline void SetAngDrag(const float drag) { angularDrag = drag; }
	inline void SetRotInertia(const float r) { rotInertia = r; }
#pragma endregion
};

#endif