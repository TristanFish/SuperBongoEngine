#include "CollisionDetection.h"

#include "core/3D/Physics/BoundingBox.h"
#include "core/3D/Physics/BoundingSphere.h"
#include "core/CoreEngine.h"
#include "core/Globals.h"

#include "components/3D/RigidBody3D.h"
#include "components/GameObject.h"
#include "Rendering/Renderer.h"

#include "Ray.h"

using namespace  MATH;

void CollisionDetection::NudgeObject(const CollisionHitInfo& HitInfo, Collider3D& Box)
{
	float NudgeAmount = HitInfo.Depth;
	float NudgeStep = 0.1;

	std::shared_ptr<GameObject> NudgedObject = Box.GetRBAttached()->gameObject;

	
	NudgedObject->transform.GetPositionRef() += ((HitInfo.Normal * -1) * NudgeStep);


	//NudgeAmount -= NudgeStep;
}

CollisionDetection::~CollisionDetection() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//Ray Collisions
//////////////////////////////////////////////////////////////////////////////////////////////////////
Ray CollisionDetection::MousePosToWorldRay(MATH::Vec2 mouseCoords_) {

	Vec2 viewportSize = Renderer::GetInstance()->GetViewport().GetViewportSize();

	Vec4 rayStartNDC((mouseCoords_.x / Globals::Engine::SCREEN_WIDTH - 0.5f) * 2.0f, (mouseCoords_.y / Globals::Engine::SCREEN_HEIGHT - 0.5f) * 2.0f, -1.0f, 1.0f);
	Vec4 rayEndNDC((mouseCoords_.x / Globals::Engine::SCREEN_WIDTH - 0.5f) * 2.0f, (mouseCoords_.y / Globals::Engine::SCREEN_HEIGHT - 0.5f) * 2.0f, 0.0f, 1.0f);

	Matrix4 inverse = MMath::inverse(Camera::getInstance()->getProjectionMatrix() * Camera::getInstance()->getViewMatrix());

	Vec4 rayStartWorld = inverse * rayStartNDC;
	rayStartWorld /= rayStartWorld.w;

	Vec4 rayEndWorld = inverse * rayEndNDC;
	rayEndWorld /= rayEndWorld.w;

	Vec3 rayDirWorld(rayEndWorld - rayStartWorld);
	rayDirWorld = VMath::normalize(rayDirWorld);

	return Ray(rayDirWorld, Vec3(rayStartWorld));
}

bool CollisionDetection::RayOBBIntersection(Ray* ray_, BoundingBox* box_) {

	Matrix4 modelMatrixrix = box_->GetTransform();

	Vec3 rayOrigin = ray_->origin;
	Vec3 rayDirection = ray_->direction;
	
	float tMin = Camera::getInstance()->getNearPlane();
	float tMax = Camera::getInstance()->getFarPlane();

	Vec3 worldPos = modelMatrixrix.getColumn(3);
	Vec3 delta = worldPos - rayOrigin;


	for (int i = 0; i < 3; i++)
	{
		Vec3 Axis = modelMatrixrix.getColumn(i);

		//X axis
		float dotDelta = VMath::dot(Axis, delta);
		float dotDir = VMath::dot(rayDirection, Axis);

		if (fabs(dotDir) > 0.001f) {
			float t1 = (dotDelta + box_->GetWorldVerticies()[0][i]) / dotDir;
			float t2 = (dotDelta + box_->GetWorldVerticies()[1][i]) / dotDir;

			if (t1 > t2) { //makes sure that t1 is the min and t2 is max, if they aren't then swap them
				float w = t1;
				t1 = t2;
				t2 = w;
			}
			if (t2 < tMax) {
				tMax = t2;
			}
			if (t1 > tMin) {
				tMin = t1;
			}
			if (tMax < tMin) {
				return false;
			}
		}
		else { //teacher question
			if (-dotDelta + box_->GetWorldVerticies()[0][i] > 0.0f || -dotDelta + box_->GetWorldVerticies()[1][i] < 0.0f) {
				return false;
			}
		}
	}
	ray_->distance = tMin;
	return true;
}

bool CollisionDetection::ColliderIntersection(Collider3D* Collider1, Collider3D* Collider2)
{
	if (Collider1->IsType(ColliderType::OBB,{Collider2}))
	{
		return OBBIntersection(dynamic_cast<BoundingBox&>(*Collider1), dynamic_cast<BoundingBox&>(*Collider2));
	}
	else if (Collider1->IsType(ColliderType::Sphere, { Collider2 }))
	{
		return SphereIntersection(dynamic_cast<BoundingSphere&>(*Collider1), dynamic_cast<BoundingSphere&>(*Collider2));
	}
}

bool CollisionDetection::OBBIntersection(BoundingBox& Box1, BoundingBox& Box2)
{
	bool Collided = false;


	if (!Box1.IsMoveable() && !Box2.IsMoveable())
	{

	}
	RigidBody3D* RB_1 = Box1.GetRBAttached();
	RigidBody3D* RB_2 = Box2.GetRBAttached();



	GJKDetection GJKDetector;

	Collided = GJKDetector.GJKCollisionDetection(Box1.GetWorldVerticies(), Box2.GetWorldVerticies());



	if (Collided) {

		CollisionHitInfo HitInfo = GJKDetector.GetRecentHitInfo();

		

		float Box1Mobility = Box1.IsMoveable() ? 1.0f : 0.0f;
		float Box2Mobility = Box2.IsMoveable() ? 1.0f : 0.0f;


		float Box1Distance = Box1Mobility * HitInfo.Depth  * RB_1->GetMass() / ((RB_1->GetMass() * Box1Mobility) + (RB_2->GetMass() * Box2Mobility));
		float Box2Distance = Box2Mobility * HitInfo.Depth  * RB_2->GetMass() / ((RB_1->GetMass() * Box1Mobility) + (RB_2->GetMass() * Box2Mobility));

		Box1.OnCollisionEnter(Box2);
		Box2.OnCollisionEnter(Box1);


		Vec3 ProjectedVel1 = HitInfo.Normal * VMath::dot(RB_1->GetVelocity(), HitInfo.Normal);
		Vec3 ProjectedVel2 = HitInfo.Normal * VMath::dot(RB_2->GetVelocity(), HitInfo.Normal);

		RB_1->SetVelocity(RB_1->GetVelocity() - ProjectedVel1);
		RB_2->SetVelocity(RB_2->GetVelocity() - ProjectedVel2);

		RB_1->gameObject->SetPos(RB_1->GetPosition() - Box1Distance  * HitInfo.Normal);

		RB_2->gameObject->SetPos(RB_2->GetPosition() + Box2Distance  * HitInfo.Normal);

		
		return Collided;
	}
	

	return Collided;
}

bool CollisionDetection::SphereIntersection(BoundingSphere& Sphere1, BoundingSphere& Sphere2)
{
	Vec3 Centre_1 = Sphere1.GetCentre();
	Vec3 Centre_2 = Sphere2.GetCentre();

	Vec3 delta = Centre_2 - Centre_1;

	float CombinedRadius = powf(Sphere1.GetRadius() + Sphere2.GetRadius(), 2.0f);

	if (VMath::dot(delta,delta) < CombinedRadius)
	{
		RigidBody3D* RB_1 = Sphere1.GetRBAttached();
		RigidBody3D* RB_2 = Sphere2.GetRBAttached();


		Vec3 DifferencePos = Centre_2 - Centre_1;

		Vec3 Dir = VMath::normalize(DifferencePos);

		Vec3 DifferenceVel = RB_2->GetVelocity() - RB_1->GetVelocity();

		float Speed = VMath::dot(DifferenceVel, Dir);

		float SphereSpeed1 = (2.0f * RB_2->GetMass() * Speed) / (RB_1->GetMass() + RB_2->GetMass());
		float Sphere2Speed = (Speed * (RB_2->GetMass() + RB_1->GetMass())) / (RB_1->GetMass() + RB_2->GetMass());
		

		Sphere1.OnCollisionEnter(Sphere2);
		RB_1->SetVelocity(Dir * SphereSpeed1);
		
		Sphere2.OnCollisionEnter(Sphere1);
		RB_2->SetVelocity(Dir * (SphereSpeed1 - Speed));

		
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////