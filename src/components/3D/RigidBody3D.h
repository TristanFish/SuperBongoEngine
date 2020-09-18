#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "components/ECS.h"
#include "math/Vector.h"
#include "components/2D/Collider.h"

class RigidBody3D : public Component
{
private:
	float mass;
	MATH::Vec3* pos; 
	MATH::Vec3 vel;
	MATH::Vec3 accel;
	float linearDrag;

	float rotInertia;
	MATH::Vec3 Angle;
	MATH::Vec3 angularVel;
	MATH::Vec3 angularAcc;
	float angularDrag;

	 bool IsGrounded;


	friend class Physics3D;
public:
	Collider collider;

	RigidBody3D();
	~RigidBody3D();

	// Inherited via Component
	void Init(GameObject *g) override;
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;

	void ApplyImpulseForce(const MATH::Vec3& force);
	void ApplyConstantForce(const MATH::Vec3& force);
	void SetConstantForce(const MATH::Vec3& force);


	void ApplyImpulseTorque(const MATH::Vec3 torque);
	void ApplyConstantTorque(const MATH::Vec3 torque);

	inline void setColliderSize(MATH::Vec3 s) { collider.size = s; }
	inline void setColliderShape(Collider::shape newShape) { collider.colliderShape = newShape; }
	inline Collider::shape GetColliderShape() { return collider.colliderShape; }

	inline bool isMoveable() { return collider.isMoveable; }
	inline void setMoveable(bool b) { collider.isMoveable = b; }

	virtual void OnCollisionEnter(RigidBodyComponent& otherBody) = 0;
	//Getters and setters
#pragma region getters/setters
	inline MATH::Vec3 GetPosition() { return *pos; }
	inline void SetPosition(MATH::Vec3 p) { *pos = p; }
	inline MATH::Vec3 GetVelocity() { return vel; }
	inline void SetVelocity(const MATH::Vec3& velocity) { vel = velocity; }
	inline MATH::Vec3 GetAccel() { return accel; }
	inline void SetAccel(const MATH::Vec3& acc) { accel = acc; }
	inline float GetLinDrag() { return linearDrag; }
	inline void SetLinDrag(const float drag) { linearDrag = drag; }
	inline void SetMass(const float m) { mass = m; }
	inline void SetIsGrounded(const bool grounded) { IsGrounded = grounded; }

	inline MATH::Vec3 GetAngVelocity() { return angularVel; }
	inline void SetAngVelocity(const MATH::Vec3 vel) { angularVel = vel; }
	inline MATH::Vec3 GetAngAccel() { return angularAcc; }
	inline void SetAngAccel(const MATH::Vec3 acc) { angularAcc = acc; }
	inline float GetAngDrag() { return angularDrag; }
	inline void SetAngDrag(const float drag) { angularDrag = drag; }
	inline bool GetIsGrounded() { return IsGrounded; }
	inline void SetRotInertia(const float r) { rotInertia = r; }
#pragma endregion
};

#endif