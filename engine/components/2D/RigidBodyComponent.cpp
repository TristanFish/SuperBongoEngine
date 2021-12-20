#include "RigidBodyComponent.h"
#include "core/Timer.h"


RigidBodyComponent::RigidBodyComponent(): mass(1.0f), vel(MATH::Vec3()), accel(MATH::Vec3()), linearDrag(0.0f), rotInertia(0.0f),
										  zAngle(0.0f), angularVel(0.0f), angularAcc(0.0f), angularDrag(0.95f)
{

}

RigidBodyComponent::~RigidBodyComponent()
{
	pos = nullptr;
}

void RigidBodyComponent::Init(GameObject *g)
{
	gameObject = g;
	pos = &g->transform.GetPosition();
	setColliderSize(g->transform.GetScale());

	mass = 1.0f;
	vel = MATH::Vec3();
	accel = MATH::Vec3();

	rotInertia = 1.0f;
	zAngle = 0.0f;
	angularVel = 0.0f;
	angularAcc = 0.0f;
}

void RigidBodyComponent::Update(const float deltaTime)
{
	vel += accel * deltaTime;
	*pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;


	angularVel += angularAcc * deltaTime;
	angularVel *= angularDrag;
	zAngle += angularVel * deltaTime + 0.5f * angularAcc * deltaTime * deltaTime;

	gameObject->SetRotation(MATH::Vec3(0.0f, 0.0f, zAngle));

}

void RigidBodyComponent::ApplyImpulseForce(const MATH::Vec3& force)
{
	vel += (force / mass) * Timer::GetScaledDeltaTime();
}

void RigidBodyComponent::SetConstantForce(const MATH::Vec3& force)
{
	accel = force / mass;
}
void RigidBodyComponent::ApplyConstantForce(const MATH::Vec3& force)
{
	accel += force / mass;
}

void RigidBodyComponent::ApplyImpulseTorque(const float torque)
{
	angularVel += (torque / rotInertia) * Timer::GetScaledDeltaTime();
}

void RigidBodyComponent::ApplyConstantTorque(const float torque)
{
	angularAcc = torque / mass;
}