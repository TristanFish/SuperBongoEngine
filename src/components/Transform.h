#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math/MMath.h"

//Not quite a component but similar enough to be in the same folder path
//gameobjects have a transform to define their position rotation and scale
class Transform
{
private:

	MATH::Matrix4 modelMatrix;
	MATH::Matrix4 rotationMatrix;
	
public:
	MATH::Vec3 pos, rotation, scale;

	Transform();
	Transform(const MATH::Vec3& pos_);

	void Update(const float deltaTime);

	inline MATH::Vec3& GetPosition() { return pos; }
	inline MATH::Vec3& GetRotation() { return rotation; }
	inline MATH::Vec3& GetScale() { return scale; }
	inline MATH::Matrix4& GetModelMatrix() { return modelMatrix; }
	inline MATH::Matrix4& GetRotationMatrix() { return rotationMatrix; }

	//This vector always points in front of the object
	MATH::Vec3 Forward() const;
	//This vector always points to the right of the object
	MATH::Vec3 Right() const;
	//This vector always points above the object
	MATH::Vec3 Up() const;

	void setPos(const MATH::Vec3& pos);
	void setRot(const MATH::Vec3& rot);

};

#endif