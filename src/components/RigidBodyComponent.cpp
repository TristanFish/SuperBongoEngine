#include "RigidBodyComponent.h"

void RigidBodyComponent::Init() 
{
	mass = 1.0f;
	pos = gameObject->transform->GetPosition();
	vel = MATH::Vec3();
	accel = MATH::Vec3();

	rotInertia = 1.0f;
	zAngle = 0.0f;
	angularVel = 0.0f;
	angularAcc = 0.0f;
}

void RigidBodyComponent::Update(const float deltaTime)
{
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;
	vel += accel * deltaTime;

	zAngle += angularVel * deltaTime + 0.5f * angularAcc * deltaTime * deltaTime;
	angularVel += angularAcc * deltaTime;
}

void RigidBodyComponent::Render() const
{
	//RigidBody doesn't do any rendering
}

void RigidBodyComponent::HandleEvents(const SDL_Event& event)
{
	//RigidBody doesn't do any event listening
}

void RigidBodyComponent::ApplyImpulseForce(const MATH::Vec3& force)
{
	vel += (force / mass) * Timer::GetScaledDeltaTime();
}

void RigidBodyComponent::ApplyConstantForce(const MATH::Vec3& force)
{
	accel = force / mass;
}

void RigidBodyComponent::ApplyImpulseTorque(const float torque)
{
	angularVel += (torque / rotInertia) * Timer::GetScaledDeltaTime();
}

void RigidBodyComponent::ApplyConstantTorque(const float torque)
{
	angularAcc = torque / mass;
}