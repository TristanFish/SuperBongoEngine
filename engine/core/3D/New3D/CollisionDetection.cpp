#include "CollisionDetection.h"

#include "BoundingBox.h"
#include "Ray.h"
#include "core/CoreEngine.h"


using namespace  MATH;

CollisionDetection::~CollisionDetection() {

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//Ray Collisions
//////////////////////////////////////////////////////////////////////////////////////////////////////
Ray CollisionDetection::MousePosToWorldRay(Vec2 mouseCoords_, float screenWidth_, float screenHeight_, Camera* camera_) {
	Vec4 rayStartNDC((mouseCoords_.x / screenWidth_ - 0.5f) * 2.0f, (mouseCoords_.y / screenHeight_ - 0.5f) * 2.0f, -1.0f, 1.0f);
	Vec4 rayEndNDC((mouseCoords_.x / screenWidth_ - 0.5f) * 2.0f, (mouseCoords_.y / screenHeight_ - 0.5f) * 2.0f, 0.0f, 1.0f);

	Matrix4 inverse = MMath::inverse(camera_->getProjectionMatrix() * camera_->getViewMatrix());

	Vec4 rayStartWorld = inverse * rayStartNDC;
	rayStartWorld /= rayStartWorld.w;

	Vec4 rayEndWorld = inverse * rayEndNDC;
	rayEndWorld /= rayEndWorld.w;

	Vec3 rayDirWorld(rayEndWorld - rayStartWorld);
	rayDirWorld = VMath::normalize(rayDirWorld);

	return Ray(Vec3(rayStartWorld), rayDirWorld);
}

bool CollisionDetection::RayObbIntersection(Ray* ray_, BoundingBox* box_) {
	Matrix4 modelMatrixrix = box_->transform;
	Vec3 rayOrigin = ray_->origin;
	Vec3 rayDirection = ray_->direction;
	Vec3 boxMin = box_->minVert;
	Vec3 boxMax = box_->maxVert;

	float tMin = Camera::getInstance()->getNearPlane();
	float tMax = Camera::getInstance()->getFarPlane();

	Vec3 worldPos(modelMatrixrix[3], modelMatrixrix[7], modelMatrixrix[11]);
	Vec3 delta = worldPos - rayOrigin;

	//X axis
	Vec3 xAxis(modelMatrixrix[0], modelMatrixrix[4], modelMatrixrix[8]);
	float dotDelta = VMath::dot(delta, xAxis);
	float dotDir = VMath::dot(rayDirection, xAxis);
	if (fabs(dotDir) > 0.001f) {
		float t1 = (dotDelta + boxMin.x) / dotDir;
		float t2 = (dotDelta + boxMax.x) / dotDir;

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
		if (-dotDelta + boxMin.x > 0.0f || -dotDelta + boxMax.x < 0.0f) {
			return false;
		}
	}

	//Y axis
	Vec3 yAxis(modelMatrixrix[1], modelMatrixrix[5], modelMatrixrix[9]);
	dotDelta = VMath::dot(delta, yAxis);
	dotDir = VMath::dot(rayDirection, yAxis);
	if (fabs(dotDir) > 0.001f) {
		float t1 = (dotDelta + boxMin.y) / dotDir;
		float t2 = (dotDelta + boxMax.y) / dotDir;

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
		if (-dotDelta + boxMin.y > 0.0f || -dotDelta + boxMax.y < 0.0f) {
			return false;
		}
	}

	//Z axis
	Vec3 zAxis(modelMatrixrix[2], modelMatrixrix[6], modelMatrixrix[10]);
	dotDelta = VMath::dot(delta, zAxis);
	dotDir = VMath::dot(rayDirection, zAxis);
	if (fabs(dotDir) > 0.001f) {
		float t1 = (dotDelta + boxMin.z) / dotDir;
		float t2 = (dotDelta + boxMax.z) / dotDir;

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
		if (-dotDelta + boxMin.z > 0.0f || -dotDelta + boxMax.z < 0.0f) {
			return false;
		}
	}

	ray_->distance = tMin;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////////////////////