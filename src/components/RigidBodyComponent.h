#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Components.h"
#include "math/Vector.h"
#include "math/VMATH.h"
#include "Timer.h"
#include "components/Collider.h"

class RigidBodyComponent : public Component
{
private:
	float mass;
	MATH::Vec3& pos; 
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

	// Inherited via Component
	virtual void Init() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;

	void ApplyImpulseForce(const MATH::Vec3& force);
	void ApplyConstantForce(const MATH::Vec3& force);

	void ApplyImpulseTorque(const float torque);
	void ApplyConstantTorque(const float torque);

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