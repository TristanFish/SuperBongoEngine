#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "VMath.h"
#include "MMath.h"

#include "Collider3D.h"


using namespace MATH;

class BoundingBox : public Collider3D {
	

private:
	Vec3 orginalMax,maxVert;
	Vec3 origionalMin,minVert;
	Matrix4 transform;

public:
	inline BoundingBox() {
		maxVert = minVert = Vec3();
		transform = Matrix4();
	}
	inline BoundingBox(const Vec3& maxVert_, const Vec3& minVert_, const Matrix4& transform_) {
		maxVert = orginalMax = maxVert_;
		minVert = origionalMin = minVert_;
		transform = transform_;
	}

	inline ~BoundingBox()
	{
	}


	 inline void SetSize(const MATH::Vec3& NewSize)  
	{ 
		V_Size = NewSize; 
	}

	 inline void UpdateModelBounds()
	 {
		 minVert = transform * origionalMin;
		 maxVert = transform * orginalMax;
	 }

	inline Matrix4& GetTransform() { return transform; }

	inline const Vec3& GetMinVertex() { return minVert; }
	inline const Vec3& GetMaxVertex() { return maxVert; }


	inline void SetMinVertex(const Vec3& newMin) { minVert = newMin; }
	inline void SetMaxVertex(const Vec3& newMax) { maxVert = newMax; }
	inline void SetTransform(const Matrix4& newMatrix) { transform = newMatrix; }

	inline Vec3 GetMinTransformedPoint()  {

		return Vec3(transform[3], transform[7], transform[11]) + minVert;
	}
	inline Vec3 GetMaxTransformedPoint() {

		return Vec3(transform[3], transform[7], transform[11]) + maxVert;

	}
};




#endif
