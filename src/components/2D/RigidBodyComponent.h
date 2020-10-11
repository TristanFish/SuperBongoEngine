#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "components/ECS.h"
#include "math/Vector.h"
#include "components/Collider.h"

class RigidBodyComponent : public Component
{
private:
	float mass;
	MATH::Vec3* pos; 
	MATH::Vec3 vel;
	MATH::Vec3 accel;
	float linearDrag;

	float rotInertia;
	float zAngle;
	float angularVel;
	float angularAcc;
	float angularDrag;

	 bool IsGrounded;


	friend class Physics2D;
public:
	Collider collider;

	RigidBodyComponent();
	~RigidBodyComponent();

	// Inherited via Component
	void Init(GameObject *g) override;
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;

	void ApplyImpulseForce(const MATH::Vec3& force);
	void ApplyConstantForce(const MATH::Vec3& force);
	void SetConstantForce(const MATH::Vec3& force);


	void ApplyImpulseTorque(const float torque);
	void ApplyConstantTorque(const float torque);

	inline void setColliderSize(float s) { collider.size = s; }
	inline void setColliderShape(Collider::shape newShape) { collider.colliderShape = newShape; }
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

	inline float GetAngVelocity() { return angularVel; }
	inline void SetAngVelocity(const float vel) { angularVel = vel; }
	inline float GetAngAccel() { return angularAcc; }
	inline void SetAngAccel(const float acc) { angularAcc = acc; }
	inline float GetAngDrag() { return angularDrag; }
	inline void SetAngDrag(const float drag) { angularDrag = drag; }
	inline bool GetIsGrounded() { return IsGrounded; }
	inline void SetRotInertia(const float r) { rotInertia = r; }
#pragma endregion
};

#endif