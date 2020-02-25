#include "RigidBodyComponent.h"
#include "math/VMATH.h"
#include "core/Timer.h"

RigidBodyComponent::RigidBodyComponent(): mass(1.0f), vel(MATH::Vec3()), accel(MATH::Vec3()), linearDrag(0.0f), rotInertia(0.0f),
										  zAngle(0.0f), angularVel(0.0f), angularAcc(0.0f), angularDrag(0.0f)
{

}

void RigidBodyComponent::Init(GameObject *g)
{
	gameobject = g;

	setColliderSize(g->transform.GetScale().x);

	mass = 1.0f;
	pos = gameobject->transform.pos;
	vel = MATH::Vec3();
	accel = MATH::Vec3();

	rotInertia = 1.0f;
	zAngle = 0.0f;
	angularVel = 0.0f;
	angularAcc = 0.0f;
}

void RigidBodyComponent::Update(const float deltaTime)
{
	pos = gameobject->transform.GetPosition();

	vel += accel * deltaTime;
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;

	gameobject->transform.setPos(pos);

	angularVel += angularAcc * deltaTime;
	zAngle += angularVel * deltaTime + 0.5f * angularAcc * deltaTime * deltaTime;

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