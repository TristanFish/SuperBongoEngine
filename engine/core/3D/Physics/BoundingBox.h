#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "VMath.h"
#include "MMath.h"

#include "Collider3D.h"


using namespace MATH;

class BoundingBox : public Collider3D {
	

private:
	Vec3 maxVert, minVert;
	Matrix4 transform;

public:
	inline BoundingBox() {
		maxVert = minVert = Vec3();
		transform = Matrix4();
	}
	inline BoundingBox(Vec3 maxVert_, Vec3 minVert_, Matrix4& transform_) {
		maxVert = maxVert_;
		minVert = minVert_;
		transform = transform_;
	}

	inline ~BoundingBox()
	{
		
	}


	 inline void SetSize(const MATH::Vec3& NewSize)  
	{ 
		V_Size = NewSize; 
	}


	inline Matrix4& GetTransform() { return transform; }

	inline Vec3 GetMinVertex() { return minVert; }
	inline Vec3 GetMaxVertex() { return maxVert; }

	inline void SetMinVertex(const Vec3& newMin) { minVert = newMin; }
	inline void SetMaxVertex(const Vec3& newMax) { maxVert = newMax; }


	inline Vec3 GetMinTransformedPoint() {

		return Vec3(transform[3], transform[7], transform[11]) + minVert;
	}
	inline Vec3 GetMaxTransformedPoint() {


		return Vec3(transform[3], transform[7], transform[11]) + maxVert;

	}
};




#endif
