#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math/Matrix.h"
#include "math/MMath.h"

class Transform
{
private:

	MATH::Matrix4 modelMatrix;
	
public:
	MATH::Vec3 pos, rotation, scale;

	Transform();

	Transform(const MATH::Vec3& pos_);

	inline MATH::Vec3& GetPosition() { return pos; }

	inline MATH::Vec3& GetRotation() { return rotation; }

	inline MATH::Vec3& GetScale() { return scale; }

	inline MATH::Matrix4& GetModelMatrix() { return modelMatrix; }

	void Update(const float deltaTime);

	void setPos(const MATH::Vec3& pos_);
};

#endif