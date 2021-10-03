#include "Physics3D.h"

#include "core/Logger.h"
#include "Rendering/Camera.h"
#include "math/Vector.h"
#include "math/VMath.h"

using namespace MATH;

bool Physics3D::BoxBoxDetect(RigidBody3D& rb1, RigidBody3D& rb2)
{
	/*
	float rb1Halfx = ((abs(rb1.collider.minVertices.x) + abs(rb1.collider.maxVertices.x)) / 2.0f);
	float rb2Halfx = ((abs(rb2.collider.minVertices.x) + abs(rb2.collider.maxVertices.x)) / 2.0f);
	float rb1Halfy = ((abs(rb1.collider.minVertices.y) + abs(rb1.collider.maxVertices.y)) / 2.0f);
	float rb2Halfy = ((abs(rb2.collider.minVertices.y) + abs(rb2.collider.maxVertices.y)) / 2.0f);
	float rb1Halfz = ((abs(rb1.collider.minVertices.z) + abs(rb1.collider.maxVertices.z)) / 2.0f);
	float rb2Halfz = ((abs(rb2.collider.minVertices.z) + abs(rb2.collider.maxVertices.z)) / 2.0f);

	if ((rb1.pos->x - rb1Halfx < rb2.pos->x + rb2Halfx) && //Check rb1 left edge with rb2 right edge
		(rb1.pos->x + rb1Halfx > rb2.pos->x - rb2Halfx) && //Check rb1 right edge with rb2 left edge
		(rb1.pos->y - rb1Halfy < rb2.pos->y + rb2Halfy) && //ChecK rb1 bottom edge with rb2 top edge
		(rb1.pos->y + rb1Halfy > rb2.pos->y - rb2Halfy) && 
		(rb1.pos->z - rb1Halfz < rb2.pos->z + rb2Halfz) &&   //Check rb2 top edge with rb2 bottom edge
		(rb1.pos->z + rb1Halfz > rb2.pos->z - rb2Halfz))
	{	
		return true;
	}
	 */


	return (abs(rb1.collider.minVertices.x) <= abs(rb2.collider.maxVertices.x) && abs(rb1.collider.maxVertices.x) >= abs(rb2.collider.minVertices.x)) &&
		(abs(rb1.collider.minVertices.y) <= abs(rb2.collider.maxVertices.y) && abs(rb1.collider.maxVertices.y) >= abs(rb2.collider.minVertices.y)) &&
		(abs(rb1.collider.minVertices.z) <= abs(rb2.collider.maxVertices.z) && abs(rb1.collider.maxVertices.z) >= abs(rb2.collider.minVertices.z));

}

bool Physics3D::SphereBoxDetect(RigidBody3D& rb1, RigidBody3D& rb2)
{
	RigidBody3D* sphere = nullptr;
	RigidBody3D* box = nullptr;

	if (rb1.GetColliderShape() == Collider3D::type::AABB)
	{
		sphere = &rb2;
		box = &rb1;
	}
	else
	{
		sphere = &rb1;
		box = &rb2;
	}


	MATH::Vec3 p_min_1 = sphere->collider.minVertices;
	MATH::Vec3 p_max_1 = sphere->collider.maxVertices;

	MATH::Vec3 p_min_2 = box->collider.minVertices;;
	MATH::Vec3 p_max_2 = box->collider.maxVertices;

	//Find the difference between both positions
	Vec3 differenceVector = *sphere->pos - *box->pos;

	//find the distance to the edge of the box and clamp the difference to find the closest contact point
	Vec3 clamped = MATH::VMath::clamp(differenceVector, box->collider.minVertices /2,
		box->collider.maxVertices /2);
	Vec3 closestContactPoint = *box->pos + clamped;

	//distance from closest contact point to the center of the circle
	Vec3 distance = closestContactPoint - *sphere->pos;

	if (VMath::mag(distance) < (abs(sphere->collider.maxVertices.x) + abs(sphere->collider.minVertices.x)) / 2 ||
		VMath::mag(distance) < (abs(sphere->collider.maxVertices.y) + abs(sphere->collider.minVertices.y)) / 2 ||
		VMath::mag(distance) < (abs(sphere->collider.maxVertices.z) + abs(sphere->collider.minVertices.z)) / 2)
	{
		sphere = nullptr;
		box = nullptr;
		return true;
	}
	
	sphere = nullptr;
	box = nullptr;
	return false;
}

bool Physics3D::BoxPlaneDetect(RigidBody3D& box, RigidBody3D& plane)
{
	
	Vec3 extents = (box.collider.maxVertices - box.collider.minVertices) / 2.0f;
	extents += box.GetPosition() * -1;
	Vec3 centre = box.collider.minVertices + extents;

	Vec3 pNormal =  plane.gameobject->transform.Up() / 2;

	float r = (pNormal.x * extents.x) + (pNormal.y * extents.y) + (pNormal.z * extents.z);

	float dot = VMath::dot(pNormal, centre) - VMath::distance(box.GetPosition(), plane.GetPosition());	



	if(dot >= r)
	{
		return false;
	}
	if (dot <= -r)
	{
		return false;
	}

	std::string inf = "Box Pane collision between ";
	inf.append(box.gameobject->name);
	inf.append(" and ");
	inf.append(plane.gameobject->name);

	EngineLogger::Info(inf, "Physics3D.cpp", __LINE__);
	return true;
}

bool Physics3D::SpherePlaneDetect(RigidBody3D& sphere, RigidBody3D& plane)
{

	//Vec3 closestPoint 
	return false;
}

bool Physics3D::RayOBBDetect(MouseRay& ray, const OrientedBoundingBox& obb)
{	
	Matrix4 modelMatrix = obb.transform;
	
	float tMin = Camera::getInstance()->getNearPlane();
	float tMax = Camera::getInstance()->getFarPlane();

	
	Vec3 worldPos(modelMatrix.getColumn(3));
	Vec3 delta = worldPos - ray.GetCurrentRay().origin;

	Vec3 xAxis(modelMatrix.getColumn(0));
	float dotDelta = VMath::dot(xAxis, delta);
	float dotDir = VMath::dot(ray.GetCurrentRay().direction, xAxis);

	if(fabs(dotDir) > 0.001f)
	{
		float t1 = (dotDelta + obb.minVert.x) / dotDir;
		float t2 = (dotDelta + obb.maxVert.x) / dotDir;

		if(t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if(t2 < tMax)
		{
			tMax = t2;
		}

		if(t1 > tMin)
		{
			tMin = t1;
		}

		if(tMax < tMin)
		{
			return false;
		}
	}
	else
	{
		if(-dotDelta + obb.minVert.x > 0.0f || -dotDelta + obb.maxVert.x < 0.0f)
		{
			return false;
		}
	}

	Vec3 yAxis(modelMatrix.getColumn(1));
	dotDelta = VMath::dot(yAxis, delta);
	dotDir = VMath::dot(ray.GetCurrentRay().direction, yAxis);
	
	if(fabs(dotDir) > 0.001f)
	{
		float t1 = (dotDelta + obb.minVert.y) / dotDir;
		float t2 = (dotDelta + obb.maxVert.y) / dotDir;

		if(t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if(t2 < tMax)
		{
			tMax = t2;
		}

		if(t1 > tMin)
		{
			tMin = t1;
		}

		if(tMax < tMin)
		{
			return false;
		}
	}
	else
	{
		if(-dotDelta + obb.minVert.y > 0.0f || -dotDelta + obb.maxVert.y < 0.0f)
		{
			return false;
		}
	}

	Vec3 zAxis(modelMatrix.getColumn(2));
	dotDelta = VMath::dot(zAxis, delta);
	dotDir = VMath::dot(ray.GetCurrentRay().direction, zAxis);
	
	if(fabs(dotDir) > 0.001f)
	{
		float t1 = (dotDelta + obb.minVert.z) / dotDir;
		float t2 = (dotDelta + obb.maxVert.z) / dotDir;

		if(t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if(t2 < tMax)
		{
			tMax = t2;
		}

		if(t1 > tMin)
		{
			tMin = t1;
		}

		if(tMax < tMin)
		{
			return false;
		}
	}
	else
	{
		if(-dotDelta + obb.minVert.z > 0.0f || -dotDelta + obb.maxVert.z < 0.0f)
		{
			return false;
		}
	}
	
	ray.intersectionDist = tMin;
	return true;
}

MATH::Vec3 Physics3D::CircleBoxClosestEdge(RigidBody3D& sphere, RigidBody3D& box)
{
	//Find the difference between both positions
	Vec3 differenceVector = *sphere.pos - *box.pos;

	//find the distance to the edge of the box and clamp the difference to find the closest contact point
	Vec3 clamped = VMath::clamp(differenceVector, box.collider.minVertices / 2, box.collider.maxVertices / 2);

	Vec3 closestContactPoint = *box.pos + clamped;

	//distance from closest contact point to the center of the circle
	Vec3 distance = closestContactPoint - *sphere.pos;


	Vec3 normal = box.gameobject->transform.rotation.ConvertToMatrix() * VMath::orthagonalize(clamped);
	//normal.z = VMath::mag(distance);

	return normal;
}

void Physics3D::SphereBoxResolve(RigidBody3D& rb1, RigidBody3D& rb2)
{
	RigidBody3D* sphere = nullptr;
	RigidBody3D* box = nullptr;

	if (rb1.GetColliderShape() == Collider3D::type::AABB)
	{
		box = &rb1;
		sphere = &rb2;
	}
	else
	{
		box = &rb2;
		sphere = &rb1;
	}

	if (sphere->collider.isMoveable && !box->collider.isTrigger)
	{
		Vec3 BoxNormal = CircleBoxClosestEdge(*sphere, *box);

		Vec3 reflectVel = VMath::reflect(sphere->vel, BoxNormal);

		sphere->SetPosition(*sphere->pos + (BoxNormal * 0.05f));
		if (VMath::dot(VMath::normalize(reflectVel), BoxNormal) > -0.1f)
		{
			sphere->vel += reflectVel * 0.5f;
		}
	}

	if (box->collider.isMoveable && !sphere->collider.isTrigger)
	{

	}
	box->OnCollisionEnter(*sphere);
	sphere->OnCollisionEnter(*box);
}

void Physics3D::BoxBoxResolve(RigidBody3D& rb1, RigidBody3D& rb2)
{
	if (rb1.collider.isMoveable)
	{
		rb1.vel.x = 0.0f;
		rb1.vel.y = 0.0f;
		rb1.vel.z = 0.0f;
		rb1.accel.x = 0.0f;
		rb1.accel.y = 0.0f;
		rb1.accel.z = 0.0f;
	}
	if (rb2.collider.isMoveable)
	{
		rb2.vel.x = 0.0f;
		rb2.vel.y = 0.0f;
		rb2.vel.z = 0.0f;
		rb2.accel.x = 0.0f;
		rb2.accel.y = 0.0f;
		rb2.accel.z = 0.0f;
	}

	//rb1.accel.print();
	rb1.OnCollisionEnter(rb2);
	rb2.OnCollisionEnter(rb1);
}

void Physics3D::BoxPlaneResolve(RigidBody3D& box, RigidBody3D& plane)
{
	if (box.collider.isMoveable)
	{
		box.vel.x = 0.0f;
		box.vel.y = 0.0f;
		box.vel.z = 0.0f;
		box.accel.x = 0.0f;
		box.accel.y = 0.0f;
		box.accel.z = 0.0f;
	}
	if (plane.collider.isMoveable)
	{
		plane.vel.x = 0.0f;
		plane.vel.y = 0.0f;
		plane.vel.z = 0.0f;
		plane.accel.x = 0.0f;
		plane.accel.y = 0.0f;
		plane.accel.z = 0.0f;
	}

	box.OnCollisionEnter(box);
	plane.OnCollisionEnter(box);
}

void Physics3D::SpherePlaneResolve(RigidBody3D& sphere, RigidBody3D& plane)
{
}

bool Physics3D::DetectCollision(RigidBody3D& rb1, RigidBody3D& rb2)
{
	switch (rb1.collider.colliderType | rb2.collider.colliderType)
	{
	case Collider3D::type::AABB:
		if (BoxBoxDetect(rb1, rb2))
		{
			BoxBoxResolve(rb1, rb2);
			return true;
		}
		return false;
		break;
	case Collider3D::type::Sphere | Collider3D::type::AABB:
		if (SphereBoxDetect(rb1, rb2))
		{
			SphereBoxResolve(rb1, rb2);
			return true;
		}
		return false;
		break;
	default:
		EngineLogger::Error("Something went wrong in DetectCollision", "Physics3D.cpp", __LINE__);
		break;
	}

	if (rb1.GetColliderShape() == Collider3D::type::AABB && rb2.GetColliderShape() == Collider3D::type::Plane)
	{
		if (BoxPlaneDetect(rb1, rb2))
		{
			BoxPlaneResolve(rb1, rb2);
			return true;
		}
		else { return false; }
	}
	if (rb1.GetColliderShape() == Collider3D::type::Plane && rb2.GetColliderShape() == Collider3D::type::AABB)
	{
		if (BoxPlaneDetect(rb2, rb1))
		{
			BoxPlaneResolve(rb2, rb1);
			return true;
		}
		else { return false; }
	}
	return false;
}

