#include "RigidBody3D.h"

#include "MMath.h"
#include "core/Timer.h"

#include "components/3D/MeshRenderer.h"

using namespace MATH;

void RigidBody3D::OnCollisionEnter(RigidBody3D& otherBody)
{
	if(collisionEnterCallback != nullptr)
	{
		collisionEnterCallback(otherBody);
	}
}

RigidBody3D::RigidBody3D(): mass(1.0f), vel(MATH::Vec3()), accel(MATH::Vec3()), linearDrag(0.0f), rotInertia(0.0f),
                            angularVel(Quaternion()), angularAcc(0.0f), angularDrag(0.95f), 
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

	rotInertia = 2.0f;
	angularVel = MATH::Quaternion(Vec4(10.0f, 0.0f, 0.0f, 0.0f));
	angularAcc = MATH::Vec3();

	//collider.maxVertices = ((MMath::calcRotationMatrix(gameobject->transform.rotation) * collider.maxVertices));
	//collider.minVertices = ((MMath::calcRotationMatrix(gameobject->transform.rotation) * collider.minVertices));
}

void RigidBody3D::Update(const float deltaTime)
{
	vel += accel * deltaTime;
	*pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;

	Vec3 obbSize;
	obbSize.x = abs(collider.minVertices.x) + abs(collider.maxVertices.x);
	obbSize.y = abs(collider.minVertices.y) + abs(collider.maxVertices.y);
	obbSize.z = abs(collider.minVertices.z) + abs(collider.maxVertices.z);


	
	
	//torque = InhertiaTensor * angularAcc;

	//Vec3 AxisRotation = VMath::normalize(VMath::cross(gameobject->transform.Up(), angularVel.GetVec3()));

	//angularVel = angularVel.Mag() * AxisRotation;

	//Quaternion newRot = (gameobject->transform.rotation) * (angularVel * gameobject->transform.rotation);

	//gameobject->transform.rotation += newRot.Normalized();
	
	//angularVel *= angularDrag;
	
	


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
	Vec3 angVel; 
	angVel += (torque / rotInertia) * Timer::GetScaledDeltaTime();

	angularVel = Quaternion(Vec4(angVel.x, angVel.y, angVel.z, 0.0f));
	
}

void RigidBody3D::ApplyConstantTorque(const Vec3& torque)
{
	angularAcc = torque / mass;
}