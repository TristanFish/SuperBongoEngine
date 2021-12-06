#include "CollisionDetection.h"

#include "core/3D/Physics/BoundingBox.h"
#include "core/3D/Physics/GJKDetection.h"
#include "core/CoreEngine.h"
#include "core/Globals.h"
#include "Ray.h"


using namespace  MATH;

CollisionDetection::~CollisionDetection() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//Ray Collisions
//////////////////////////////////////////////////////////////////////////////////////////////////////
Ray CollisionDetection::MousePosToWorldRay(MATH::Vec2 mouseCoords_, Camera* camera_) {
	Vec4 rayStartNDC((mouseCoords_.x / Globals::SCREEN_WIDTH - 0.5f) * 2.0f, (mouseCoords_.y / Globals::SCREEN_HEIGHT - 0.5f) * 2.0f, -1.0f, 1.0f);
	Vec4 rayEndNDC((mouseCoords_.x / Globals::SCREEN_WIDTH - 0.5f) * 2.0f, (mouseCoords_.y / Globals::SCREEN_HEIGHT - 0.5f) * 2.0f, 0.0f, 1.0f);

	Matrix4 inverse = MMath::inverse(camera_->getProjectionMatrix() * camera_->getViewMatrix());

	Vec4 rayStartWorld = inverse * rayStartNDC;
	rayStartWorld /= rayStartWorld.w;

	Vec4 rayEndWorld = inverse * rayEndNDC;
	rayEndWorld /= rayEndWorld.w;

	Vec3 rayDirWorld(rayEndWorld - rayStartWorld);
	rayDirWorld = VMath::normalize(rayDirWorld);

	return Ray(Vec3(rayStartWorld), rayDirWorld);
}

bool CollisionDetection::RayOBBIntersection(Ray* ray_, BoundingBox* box_) {

	Matrix4 modelMatrixrix = box_->GetTransform();

	Vec3 rayOrigin = ray_->origin;
	Vec3 rayDirection = ray_->direction;
	

	Vec3 RightDir(modelMatrixrix[0], modelMatrixrix[1], modelMatrixrix[2]);
	Vec3 UpDir(modelMatrixrix[4], modelMatrixrix[5], modelMatrixrix[6]);
	Vec3 ForwardDir(modelMatrixrix[8], modelMatrixrix[9], modelMatrixrix[10]);


	float tMin = Camera::getInstance()->getNearPlane();
	float tMax = Camera::getInstance()->getFarPlane();

	Vec3 worldPos(modelMatrixrix[3], modelMatrixrix[7], modelMatrixrix[11]);
	Vec3 delta = worldPos - rayOrigin;

	//X axis
	float dotDelta = VMath::dot(RightDir, delta);
	float dotDir = VMath::dot(rayDirection, RightDir);
	if (fabs(dotDir) > 0.001f) {
		float t1 = (dotDelta + box_->GetWorldVerticies()[0].x) / dotDir;
		float t2 = (dotDelta + box_->GetWorldVerticies()[1].x) / dotDir;

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
		if (-dotDelta + box_->GetWorldVerticies()[0].x > 0.0f || -dotDelta + box_->GetWorldVerticies()[1].x < 0.0f) {
			return false;
		}
	}

	//Y axis
	dotDelta = VMath::dot(UpDir, delta);
	dotDir = VMath::dot(rayDirection, UpDir);
	if (fabs(dotDir) > 0.001f) {
		float t1 = (dotDelta + box_->GetWorldVerticies()[0].y) / dotDir;
		float t2 = (dotDelta + box_->GetWorldVerticies()[1].y) / dotDir;

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
		if (-dotDelta + box_->GetWorldVerticies()[0].y > 0.0f || -dotDelta + box_->GetWorldVerticies()[1].y < 0.0f) {
			return false;
		}
	}

	//Z axis
	dotDelta = VMath::dot(RightDir, delta);
	dotDir = VMath::dot(rayDirection, RightDir);
	if (fabs(dotDir) > 0.001f) {
		float t1 = (dotDelta + box_->GetWorldVerticies()[0].z) / dotDir;
		float t2 = (dotDelta + box_->GetWorldVerticies()[1].z) / dotDir;

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
		if (-dotDelta + box_->GetWorldVerticies()[0].z > 0.0f || -dotDelta + box_->GetWorldVerticies()[1].z < 0.0f) {
			return false;
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
}

bool CollisionDetection::OBBIntersection(BoundingBox& Box1, BoundingBox& Box2)
{
	bool Collided = false;

	if (Box1.GetWorldVerticies().size() == 2 || Box2.GetWorldVerticies().size() == 2)
	{

		Vec3 minCorner = Box1.GetWorldVerticies()[0];
		Vec3 maxCorner = Box1.GetWorldVerticies()[1];

		Vec3 otherMinCorner = Box2.GetWorldVerticies()[0];
		Vec3 otherMaxCorner = Box2.GetWorldVerticies()[1];



		if ((minCorner.x <= otherMaxCorner.x && maxCorner.x >= otherMinCorner.x) &&
			(minCorner.y <= otherMaxCorner.y && maxCorner.y >= otherMinCorner.y) &&
			(minCorner.z <= otherMaxCorner.z && maxCorner.z >= otherMinCorner.z)) 
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

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////