#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math/MMath.h"
#include "math/Quaternion.h"
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

	//!rotationMatrix Matrix4
	/*! The transforms Rotation Matrix */
	MATH::Matrix4 rotationMatrix;
	
	void UpdateMatricies();

	//!Standard Transform Vec3s
	/*! Stores the main transform variables */
	MATH::Vec3 pos, scale;

	MATH::Quaternion rotation;

public:
	Transform* parent;
	




	//!Transform Constructor
	/*! Initializes the rotational Matrix */
	Transform();

	//!Alternate Transform Constructor
	/*! Initializes the rotational Matrix */
	Transform(const MATH::Vec3& pos_);


	//!Update Function
	/*! Updates the transforms variables */
	void Update(const float deltaTime);

	//!GetPosition Getter
	/*! Returns the transforms position */
	MATH::Vec3& GetPosition() { return pos; }
	const MATH::Vec3& GetPosition() const { return pos; }




	MATH::Quaternion GetQuaternion() const { return rotation; }

	/*! Returns the transforms rotation in the form of euler angles */
	MATH::Vec3 GetRotation() { return MATH::Quaternion::QuatToEuler(rotation); }
	const MATH::Vec3& GetRotation() const { return MATH::Quaternion::QuatToEuler(rotation); }


	//!GetScale Getter
	/*! Returns the transforms scale */
	MATH::Vec3& GetScale() { return scale; }
	const MATH::Vec3& GetScale() const { return scale; }


	MATH::Quaternion& GetRotationQuat() { return rotation; }
	const MATH::Quaternion& GetRotationQuat() const { return rotation; }


	//!GetModelMatrix Getter
	/*! Returns the transforms model matrix */
	MATH::Matrix4& GetModelMatrix()  { return modelMatrix; }

	//!GetRotationMatrix Getter
	/*! Returns the transforms rotation matrix */
	const MATH::Matrix4& GetRotationMatrix() const { return rotationMatrix; }

	void SetParent(Transform* parentTrans) { parent = parentTrans; }
	
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
	void SetPos(const MATH::Vec3& pos);

	//!SetRotation Setter
	/*! Sets the rotation of the transform */
	void SetRot(const MATH::Vec3& rot);

	void SetRot(const MATH::Quaternion& rot);

	//!SetScale Setter
	/*! Sets the rotation of the transform */
	void SetScale(const MATH::Vec3& scale);
};

#endif