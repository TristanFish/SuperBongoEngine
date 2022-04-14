#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math/MMath.h"
#include "math/Quaternion.h"

//#include <cereal/archives/json.hpp> - is already included through quat->Vec

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
	MATH::Vec3& GetPositionRef() { return pos; }
	MATH::Vec3 GetPosition() const { return pos; }

	//!GetRotation Getter
	/*! Returns the transforms rotation inthe form of euler angles */
	MATH::Vec3 GetRotation() const { return MATH::Quaternion::QuatToEuler(rotation); }

	//!GetScale Getter
	/*! Returns the transforms scale */
	MATH::Vec3& GetScaleRef() { return scale; }
	MATH::Vec3 GetScale() const { return scale; }


	MATH::Quaternion& GetRotationQuatRef() { return rotation; }
	MATH::Quaternion GetRotationQuat() const { return rotation; }


	//!GetModelMatrix Getter
	/*! Returns the transforms model matrix */
	MATH::Matrix4& GetModelMatrix()  { return modelMatrix; }


	//!GetModelMatrix Getter
	/*! Returns the transforms model matrix */
	void SetModelMatrixRow(MATH::Vec4 newRow, int index) { modelMatrix.setRow(newRow,index); }

	//!GetRotationMatrix Getter
	/*! Returns the transforms rotation matrix */
	const MATH::Matrix4& GetRotationMatrix() const { return rotationMatrix; }

	void SetParent(Transform* parentTrans) { parent = parentTrans; }
	
	//!Forward Vec3
	/*! Vector that always points in front of the transform */
	MATH::Vec3 Forward() const;
	//!Right Vec3
	/*! Vector that always points to the right of the transform */
	MATH::Vec3 Right() const;

	//!Up Vec3
	/*! Vector that always points above the transform */
	MATH::Vec3 Up() const;


	void SetPos(const MATH::Vec3& pos);

	//Sets rotation of the transform based on euler angles
	void SetRot(const MATH::Vec3& rot);
	//Sets rotation of the transform based on a quaternion
	void SetRot(const MATH::Quaternion& rot);

	void SetScale(const MATH::Vec3& scale);

	//Serialize the main parts of Transform
	template<class Archive>
	void serialize(Archive& archive) {
		archive(cereal::make_nvp("pos", pos), cereal::make_nvp("rotation", rotation), cereal::make_nvp("scale", scale));
	}
};

#endif