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
                            angularVel(Vec3()), angularAcc(0.0f), angularDrag(0.95f), 
                            collider(MATH::Vec3(),MATH::Vec3())
{
	
}

RigidBody3D::~RigidBody3D()
{
	pos = nullptr;
}

void RigidBody3D::Init(GameObject *g)
{
	gameObject = g;
	pos = &g->transform.pos;
	collider.minVertices = gameObject->GetComponent<MeshRenderer>()->GetMinVector();
	collider.maxVertices = gameObject->GetComponent<MeshRenderer>()->GetMaxVector();
	SetColliderSize(g->transform.GetScale());
	
	mass = 1.0f;
	vel = MATH::Vec3();
	accel = MATH::Vec3();

	rotInertia = 2.0f;
	angularVel = MATH::Vec3(0.0f);
	//angularAcc = MATH::Vec3(0.0f, 0.0f, 0.0f);

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


	

	angularVel += angularAcc * deltaTime;
	angularVel = angularVel * angularDrag;


	// Rotation Handling 
	Vec3 AxisRot = VMath::cross(gameObject->transform.Up(), vel);
	Quaternion newRot =  (Quaternion(Vec4(angularVel.x, angularVel.y, angularVel.z, 0.0f) * 0.5) * (gameObject->transform.rotation)) * (deltaTime / 2);



	gameObject->transform.rotation += newRot;
	gameObject->transform.rotation = gameObject->transform.rotation.Normalized();
	// Rotation Handling 
	
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
	angularVel += (torque / rotInertia) * Timer::GetScaledDeltaTime();
}

void RigidBody3D::ApplyConstantTorque(const Vec3& torque)
{
	angularAcc = torque / mass;
}