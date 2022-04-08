#include "RigidBody3D.h"

#include "MMath.h"
#include "core/Timer.h"
#include "core/3D/Physics/BoundingBox.h"
#include "core/3D/Physics/BoundingSphere.h"

#include "components/3D/MeshRenderer.h"
#include "components/GameObject.h"
#include "components/SceneGraph.h"

#include "core/Globals.h"

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

void RigidBody3D::Init(std::shared_ptr<GameObject> g)
{
	gameObject = g;
	pos = &g->transform.GetPositionRef();



	
	

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

	gameObject->transform.GetRotationQuatRef() += newRot;
	gameObject->transform.SetRot(gameObject->transform.GetRotationQuat().Normalized());
}



void RigidBody3D::HandleEvents(const SDL_Event& event)
{
	//RigidBody doesn't do any event listening
}

void RigidBody3D::ImGuiRender()
{
	const bool opened = Globals::Editor::OpenComponentTreeNode(this, "RigidBody3D");
	if (opened)
	{
		Globals::Editor::DrawVec3("Velocity", vel, 100.0f);
		Globals::Editor::DrawVec3("Acceleration", accel, 100.0f);
		Globals::Editor::DrawVec3("Angular Vel", angularVel, 100.0f);
		Globals::Editor::DrawVec3("Angular Accel", angularAcc, 100.0f);

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

void RigidBody3D::ConstructCollider(ColliderType Collider_Type)
{
	Model* objModel = gameObject->GetComponent<MeshRenderer>()->GetModel();
	Matrix4 ModelMatrix = gameObject->transform.GetModelMatrix();


#pragma region Change In Collider Type
	// Used for when we change collider types and need to copy the EnterCallback
	std::function<void(Collider3D&)> tempCollisionEnterCallback;
	bool ColliderChanged = false;

	if (collider && collider->Type != Collider_Type)
	{
		tempCollisionEnterCallback = collider->collisionEnterCallback;

		delete collider;
		collider = nullptr;

		ColliderChanged = true;
	}

#pragma endregion

	

#pragma region Create New Collider

	if (Collider_Type == ColliderType::OBB)
	{
		collider = new BoundingBox(ModelMatrix);
		collider->SetModelVerticies(objModel->GetVerticies());
		dynamic_cast<BoundingBox*>(collider)->UpdateWorldVerticies();
	}
	else if (Collider_Type == ColliderType::Sphere)
	{
		collider = new BoundingSphere(objModel->p_min, objModel->p_max);
		collider->SetModelVerticies(objModel->GetVerticies());

		dynamic_cast<BoundingSphere*>(collider)->CaclulateProperties(ModelMatrix);
	}

	if (ColliderChanged)
	{
		collider->AddCollisionFunction(tempCollisionEnterCallback);
	}


	collider->RB_Attached = this;
	collider->SetColliderType(Collider_Type);

#pragma endregion
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
	Matrix4 ModelMatrix = gameObject->transform.GetModelMatrix();

	Model* ObjModel = gameObject->GetComponent<MeshRenderer>()->GetModel();

	if (collider->GetColliderType() == ColliderType::OBB)
	{
		BoundingBox* OBB = dynamic_cast<BoundingBox*>(collider);
		OBB->SetTransform(ModelMatrix);
		OBB->UpdateModelBounds();
	}

	else if (collider->GetColliderType() == ColliderType::Sphere)
	{
		BoundingSphere* OSphere = dynamic_cast<BoundingSphere*>(collider);
		OSphere->CaclulateProperties(ModelMatrix);
	}

	

	return collider;
}
