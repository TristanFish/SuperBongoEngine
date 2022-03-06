#include "CollisionDetection.h"

#include "core/3D/Physics/BoundingBox.h"
#include "core/3D/Physics/BoundingSphere.h"
#include "core/3D/Physics/GJKDetection.h"
#include "core/CoreEngine.h"
#include "core/Globals.h"

#include "Rendering/Renderer.h"

#include "Ray.h"

using namespace  MATH;

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

void CollisionDetection::ColliderIntersection(Collider3D* Collider1, Collider3D* Collider2)
{
	if (Collider1->IsType(ColliderType::OBB,{Collider2}))
	{
		OBBIntersection(dynamic_cast<BoundingBox&>(*Collider1), dynamic_cast<BoundingBox&>(*Collider2));
	}
	else if (Collider1->IsType(ColliderType::Sphere, { Collider2 }))
	{
		SphereIntersection(dynamic_cast<BoundingSphere&>(*Collider1), dynamic_cast<BoundingSphere&>(*Collider2));
	}
}

bool CollisionDetection::OBBIntersection(BoundingBox& Box1, BoundingBox& Box2)
{
	bool Collided = false;

	if (Box1.GetColliderType() == ColliderType::OBB && Box2.GetColliderType() == ColliderType::OCTNODE)
	{

		Vec3 minCorner = Box1.GetWorldVerticies()[0];
		Vec3 maxCorner = Box1.GetWorldVerticies()[1];

		Vec3 otherMinCorner = Box2.GetWorldVerticies()[0];
		Vec3 otherMaxCorner = Box2.GetWorldVerticies()[1];



		if ((maxCorner.x >= otherMinCorner.x && minCorner.x <= otherMaxCorner.x) &&
			(maxCorner.y >= otherMinCorner.y && minCorner.y <= otherMaxCorner.y) &&
			(maxCorner.z >= otherMinCorner.z && minCorner.z <= otherMaxCorner.z))
		{
			return true;
		}
	}
	else
	{
		GJKDetection GJKDetector;

		Collided = GJKDetector.GJKCollisionDetection(Box1.GetWorldVerticies(), Box2.GetWorldVerticies());



		if (Collided) {

			Box1.OnCollisionEnter(Box2);
			Box2.OnCollisionEnter(Box1);
			return Collided;
		}
	}
	

	return Collided;
}

bool CollisionDetection::SphereIntersection(BoundingSphere& Sphere1, BoundingSphere& Sphere2)
{
	Vec3 Centre_1 = Sphere1.GetCentre();
	Vec3 Centre_2 = Sphere2.GetCentre();

	float CentrePoints = powf(Centre_1.x - Centre_2.x, 2.0f) + powf(Centre_1.y- Centre_2.y, 2.0f) + powf(Centre_1.z - Centre_2.z, 2.0f);

	float CombinedRadius = powf(Sphere1.GetRadius() + Sphere2.GetRadius(), 2.0f);

	if (CentrePoints < CombinedRadius)
	{
		Sphere1.OnCollisionEnter(Sphere2);
		Sphere2.OnCollisionEnter(Sphere1);
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////