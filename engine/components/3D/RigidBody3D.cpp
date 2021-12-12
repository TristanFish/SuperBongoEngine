#include "RigidBody3D.h"

#include "MMath.h"
#include "core/Timer.h"
#include "core/3D/Physics/BoundingBox.h"

#include "components/3D/MeshRenderer.h"

#include "graphics/UIStatics.h"

using namespace MATH;


RigidBody3D::RigidBody3D(): mass(1.0f), vel(Vec3()), accel(Vec3()), linearDrag(0.0f), rotInertia(0.0f),
                            angularVel(Vec3()), angularAcc(0.0f), angularDrag(0.95f), collider(nullptr)
{
	
}

RigidBody3D::~RigidBody3D()
{
	pos = nullptr;
	if(collider)
	{
		delete collider;
		collider = nullptr;
	}
}

void RigidBody3D::Init(GameObject *g)
{
	gameObject = g;
	pos = &g->transform.GetPositionRef();


	collider = g->GetComponent<MeshRenderer>()->MakeBoundingBox();
	SetColliderSize(g->transform.GetScale());
	collider->RB_Attached = this;

	
	

	mass = 1.0f;
	vel = Vec3();
	accel = Vec3();

	rotInertia = 2.0f;
	angularVel = Vec3(0.0f);
	angularAcc = Vec3(0.0f);
}

void RigidBody3D::Update(const float deltaTime)
{
	vel += accel * deltaTime;
	*pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;

	angularVel += angularAcc * deltaTime;
	angularVel *= angularDrag;

	// Rotation Handling 
	Vec3 AxisRot = VMath::cross(gameObject->transform.Up(), vel);
	const Quaternion newRot =  (Quaternion(Vec4(angularVel.x, angularVel.y, angularVel.z, 0.0f) * 0.5) * (gameObject->transform.GetRotationQuat())) * (deltaTime / 2);

	gameObject->transform.GetRotationQuat() += newRot;
	gameObject->transform.SetRot(gameObject->transform.GetRotationQuat().Normalized());
}



void RigidBody3D::HandleEvents(const SDL_Event& event)
{
	//RigidBody doesn't do any event listening
}

void RigidBody3D::ImGuiRender()
{
	const bool opened = UIStatics::OpenComponentTreeNode(this, "RigidBody3D");
	if (opened)
	{
		UIStatics::DrawVec3("Velocity", vel, 100.0f);
		UIStatics::DrawVec3("Acceleration", accel, 100.0f);
		UIStatics::DrawVec3("Angular Vel", angularVel, 100.0f);
		UIStatics::DrawVec3("Angular Accel", angularAcc, 100.0f);

		ImGui::TreePop();
	}
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

void RigidBody3D::SetColliderSize(MATH::Vec3 s)
{
	collider->SetSize(s);
}

void RigidBody3D::SetColliderType(ColliderType newType)
{
	collider->SetColliderType(newType); 
}

ColliderType RigidBody3D::GetColliderType()
{
	return collider->GetColliderType(); 
}

bool RigidBody3D::isMoveable() const
{
	return collider->IsMoveable(); 
}

void RigidBody3D::setMoveable(bool b)
{
	collider->SetMoveable(b); 
}

Collider3D* RigidBody3D::GetCollider()
{
	BoundingBox* OBB = dynamic_cast<BoundingBox*>(collider);

	OBB->SetTransform(gameObject->transform.GetModelMatrix());
	OBB->UpdateModelBounds();

	return collider;
}
