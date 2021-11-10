#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"
#include "MMath.h"

namespace MATH
{
	struct Quaternion
	{
	private:
		Vec4 quat;
	public:
		Quaternion(float real, const Vec3& imaginary);
		Quaternion(float x, float y, float z, float w);
		Quaternion();
		Quaternion(const Vec3& axis, float degrees);
		Quaternion(const Vec4& v);

		float Mag() const;
		Quaternion Normalized() const;
		Quaternion Conjugate() const;
		Quaternion Inverse() const;
		Vec3 Rotate(const Vec3& vec) const;
		Vec3 GetVec3() const;
		Vec4 GetQuat() const;
		Matrix3 ConvertToMatrix() const;
		static Quaternion EulerToQuat(Vec3 v);
		static Vec3 QuatToEuler(Quaternion q);
		static Quaternion LookAt(const Quaternion& currentOrientation, const Vec3& at, const Vec3& up);

		Quaternion operator * (const Quaternion& q) const;

		Quaternion& operator = (const Quaternion& q);
		Quaternion operator += (const Quaternion& q);
		Quaternion operator + (const Quaternion& q) const;


		Quaternion operator * (const Vec4& v) const;
		Quaternion operator * (const float f) const;
		Quaternion operator + (const float f) const;

		Quaternion& operator = (const Vec3& q);

		Quaternion operator += (const Vec3& v);

		Quaternion operator - () const;
	};

	std::ostream& operator<<(std::ostream& out, const Quaternion& q);
}
#endif