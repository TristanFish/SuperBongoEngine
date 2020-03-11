#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "components/ECS.h"
#include "math/Vector.h"
#include "components/Collider.h"

class RigidBodyComponent : public Component
{
private:
	float mass;
	MATH::Vec3 pos; 
	MATH::Vec3 vel;
	MATH::Vec3 accel;
	float linearDrag;

	float rotInertia;
	float zAngle;
	float angularVel;
	float angularAcc;
	float angularDrag;

	friend class Physics;
public:
	Collider collider;

	RigidBodyComponent();

	// Inherited via Component
	void Init(GameObject *g) override;
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;

	void ApplyImpulseForce(const MATH::Vec3& force);
	void ApplyConstantForce(const MATH::Vec3& force);

	void ApplyImpulseTorque(const float torque);
	void ApplyConstantTorque(const float torque);

	inline void setColliderSize(float s) { collider.size = s; }
	inline void setColliderShape(Collider::shape newShape) { collider.colliderShape = newShape; }
	inline bool isMoveable() { return collider.isMoveable; }
	inline void setMoveable(bool b) { collider.isMoveable = b; }

	virtual void OnCollisionEnter() = 0;
	//Getters and setters
#pragma region getters/setters
	inline MATH::Vec3 GetVelocity() { return vel; }
	inline MATH::Vec3 GetAccel() { return accel; }
	inline float GetLinDrag() { return linearDrag; }
	inline void SetMass(const float m) { mass = m; }
	inline void SetVelocity(const MATH::Vec3& velocity) { vel = velocity; }
	inline void SetAccel(const MATH::Vec3& acc) { accel = acc; }
	inline void SetLinDrag(const float drag) { linearDrag = drag; }

	inline float GetAngVelocity() { return angularVel; }
	inline float GetAngAccel() { return angularAcc; }
	inline float GetAngDrag() { return angularDrag; }
	inline void SetRotInertia(const float r) { rotInertia = r; }
	inline void SetAngVelocity(const float vel) { angularVel = vel; }
	inline void SetAngAccel(const float acc) { angularAcc = acc; }
	inline void SetAngDrag(const float drag) { angularDrag = drag; }
#pragma endregion
};

#endif