#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <functional>
#include "components/ECS.h"
#include "math/Vector.h"
#include "components/3D/Collider3D.h"
#include "components/3D/MeshRenderer.h"

//! RigidBody3D Class
/*! Is a component and handles almost all of the needed math for the physics of our engine */
class RigidBody3D : public Component
{
private:

	//! Float Mass
	/*! Is the mass of our object */
	float mass;

	//! Vector3 Pointer Position
	/*! Is the position of our object in world space */
	MATH::Vec3* pos;

	//! Vector3 Velocity
	/*! Is the velocity of our object  */
	MATH::Vec3 vel;

	//! Vector3 Acceleration
	/*! Is the acceleration of our object  */
	MATH::Vec3 accel;

	//! Float Linear Drag
	/*! Is the linear drag of our object  */
	float linearDrag;

	//! Float Rotational Inertia 
	/*! Is the rotational inertia of our object  */
	float rotInertia;

	

	//! Vector3 Angular Velocity 
	/*! Is the angular velocity draw of our object  */
	MATH::Vec3 angularVel;

	//! Vector3 Angular Acceleration 
	/*! Is the angular acceleration draw of our object  */
	MATH::Vec3 angularAcc;

	//! Float Angular Drag 
	/*! Is the angular drag draw of our object  */
	float angularDrag;

	//! Friend class Physics3D
	/*! Allows us to access all the variables from Physics3D */
	friend class Physics3D;

	
public:
	std::function<void(RigidBody3D&)> collisionEnterCallback;
	void OnCollisionEnter(RigidBody3D& otherBody);
	
	void AddCollisionFunction(void(*func)(RigidBody3D&))
	{
		collisionEnterCallback = func;
	}

	//! Collider3D Collider
	/*! Every RigidBody3D has a collider that allows it to collide with objects */
	Collider3D collider;

	//! RigidBody3D Constructor
	RigidBody3D();

	//! RigidBody3D Destructor
	~RigidBody3D();
	
	// Inherited via Component
	void Init(GameObject *g) override;
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;
	const char* ComponentName() const override;

	void ApplyImpulseForce(const MATH::Vec3& force);
	void ApplyConstantForce(const MATH::Vec3& force);
	void SetConstantForce(const MATH::Vec3& force);

	void ApplyImpulseTorque(const MATH::Vec3& torque);
	void ApplyConstantTorque(const MATH::Vec3& torque);

	inline void setColliderSize(MATH::Vec3 s) { collider.size = s; }
	inline void setColliderShape(Collider3D::shape newShape) { collider.colliderShape = newShape; }
	inline Collider3D::shape GetColliderShape() const { return collider.colliderShape; }

	inline bool isMoveable() const { return collider.isMoveable; }
	inline void setMoveable(bool b) { collider.isMoveable = b; }

	
	//Getters and setters
#pragma region getters/setters
	inline MATH::Vec3 GetPosition() const { return *pos; }
	inline void SetPosition(const MATH::Vec3& p) const { *pos = p; }
	inline MATH::Vec3 GetVelocity() const { return vel; }
	inline void SetVelocity(const MATH::Vec3& velocity) { vel = velocity; }
	inline MATH::Vec3 GetAccel() const { return accel; }
	inline void SetAccel(const MATH::Vec3& acc) { accel = acc; }
	inline float GetLinDrag() const { return linearDrag; }
	inline void SetLinDrag(const float drag) { linearDrag = drag; }
	inline void SetMass(const float m) { mass = m; }

	inline MATH::Vec3 GetAngVelocity() const { return angularVel; }
	inline void SetAngVelocity(const MATH::Vec3& vel) { angularVel = vel; }
	inline MATH::Vec3 GetAngAccel() const { return angularAcc; }
	inline void SetAngAccel(const MATH::Vec3& acc) { angularAcc = acc; }
	inline float GetAngDrag() const { return angularDrag; }
	inline void SetAngDrag(const float drag) { angularDrag = drag; }
	inline void SetRotInertia(const float r) { rotInertia = r; }
#pragma endregion
};

#endif