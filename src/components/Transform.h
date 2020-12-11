#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math/MMath.h"


//! Transform Class
/*! 
Not quite a component but similar enough to be in the same folder path
gameobjects have a transform to define their position rotation and scale*/
class Transform
{
private:

	//!modelMatrix Matrix4
	/*! The transforms Model Matrix */
	MATH::Matrix4 modelMatrix;
	
public:
	//!rotationMatrix Matrix4
	/*! The transforms Rotation Matrix */
	MATH::Matrix4 rotationMatrix;

	//!Standard Transform Vec3s
	/*! Stores the main transform variables */
	MATH::Vec3 pos, rotation, scale;

	//!Transform Constructor
	/*! Initializes the rotational Matrix */
	Transform();

	//!Alternate Transform Constructor
	/*! Initializes the rotational Matrix */
	Transform(const MATH::Vec3& pos_);


	//!Update Function
	/*! Updates the transforms variables */
	void Update(const float deltaTime);

	//!inline GetPosition Getter
	/*! Returns the transforms position */
	inline MATH::Vec3& GetPosition() { return pos; }

	//!inline GetRotation Getter
	/*! Returns the transforms rotation */
	inline MATH::Vec3& GetRotation() { return rotation; }

	//!inline GetScale Getter
	/*! Returns the transforms scale */
	inline MATH::Vec3& GetScale() { return scale; }

	//!inline GetModelMatrix Getter
	/*! Returns the transforms model matrix */
	inline MATH::Matrix4& GetModelMatrix() { return modelMatrix; }

	//!inline GetRotationMatrix Getter
	/*! Returns the transforms rotation matrix */
	inline MATH::Matrix4& GetRotationMatrix() { return rotationMatrix; }

	//!Forward Vec3
	/*! Vector that always points in front of the object */
	MATH::Vec3 Forward() const;
	//!Right Vec3
	/*! Vector that always points to the right of the object */
	MATH::Vec3 Right() const;

	//!Up Vec3
	/*! Vector that always points above the object */
	MATH::Vec3 Up() const;

	//!SetPosition Setter
	/*! Sets the position of the transform */
	void setPos(const MATH::Vec3& pos);

	//!SetRotation Setter
	/*! Sets the rotation of the transform */
	void setRot(const MATH::Vec3& rot);

};

#endif