#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "VMath.h"
#include "MMath.h"



using namespace MATH;

struct BoundingBox {
	
	Vec3 maxVert, minVert;
	Matrix4 transform;

	inline BoundingBox() {
		maxVert = minVert = Vec3();
		transform = Matrix4();
	}

	inline BoundingBox(Vec3 maxVert_, Vec3 minVert_, Matrix4 transform_) {
		maxVert = maxVert_;
		minVert = minVert_;
		transform = transform_;
	}

	inline bool Intersects(BoundingBox* box_) {
		Vec3 minCorner = GetTransformedPoint(minVert, transform);
		Vec3 maxCorner = GetTransformedPoint(maxVert, transform);

		Vec3 otherMinCorner = GetTransformedPoint(box_->minVert, box_->transform);
		Vec3 otherMaxCorner = GetTransformedPoint(box_->maxVert, box_->transform);

		
		if ((minCorner.x <= otherMaxCorner.x && maxCorner.x >= otherMinCorner.x) &&
			(minCorner.y <= otherMaxCorner.y && maxCorner.y >= otherMinCorner.y) &&
			(minCorner.z <= otherMaxCorner.z && maxCorner.z >= otherMinCorner.z)) {
			return true;
		}

		return false;
	}


private:
	inline Vec3 GetTransformedPoint(Vec3 point_, Matrix4 transform_) {
		return Vec3(transform_[3], transform_[7], transform_[11]) + point_;
	}
};




#endif
