#include "RigidBody3D.h"

#include "MMath.h"
#include "core/Timer.h"


using namespace MATH;

void RigidBody3D::OnCollisionEnter(RigidBody3D& otherBody)
{
	if(collisionEnterCallback != nullptr)
	{
		collisionEnterCallback(otherBody);
	}
}

RigidBody3D::RigidBody3D(): mass(1.0f), vel(MATH::Vec3()), accel(MATH::Vec3()), linearDrag(0.0f), rotInertia(0.0f),
                            angularVel(0.0f), angularAcc(0.0f), angularDrag(0.95f), 
                            collider(MATH::Vec3(),MATH::Vec3())
{
	
}

RigidBody3D::~RigidBody3D()
{
	pos = nullptr;
}

void RigidBody3D::Init(GameObject *g)
{
	gameobject = g;
	pos = &g->transform.pos;
	collider.minVertices = gameobject->GetComponent<MeshRenderer>()->GetMinVector();
	collider.maxVertices = gameobject->GetComponent<MeshRenderer>()->GetMaxVector();
	SetColliderSize(g->transform.GetScale());
	
	mass = 1.0f;
	vel = MATH::Vec3();
	accel = MATH::Vec3();

	rotInertia = 1.0f;
	angularVel = MATH::Vec3();
	angularAcc = MATH::Vec3();

	collider.maxVertices = ((MMath::calcRotationMatrix(gameobject->transform.rotation) * collider.maxVertices));
	collider.minVertices = ((MMath::calcRotationMatrix(gameobject->transform.rotation) * collider.minVertices));
}

void RigidBody3D::Update(const float deltaTime)
{
	vel += accel * deltaTime;
	*pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;

	angularVel += angularAcc * deltaTime;
	angularVel *= angularDrag;
	
}

void RigidBody3D::Render() const
{
	//RigidBody doesn't do any rendering
}

void RigidBody3D::HandleEvents(const SDL_Event& event)
{
	//RigidBody doesn't do any event listening
}

void RigidBody3D::ApplyImpulseForce(const Vec3& force)
{
	vel += (force / mass) * Timer::GetScaledDeltaTime();
}

void RigidBody3D::SetConstantForce(const Vec3& force)
{
	accel = force / mass;
}
void RigidBody3D::ApplyConstantForce(const Vec3& force)
{
	accel += force / mass;
}

void RigidBody3D::ApplyImpulseTorque(const Vec3& torque)
{
	angularVel  += (torque / rotInertia) * Timer::GetScaledDeltaTime();
}

void RigidBody3D::ApplyConstantTorque(const Vec3& torque)
{
	angularAcc = torque / mass;
}