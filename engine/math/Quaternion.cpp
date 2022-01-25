#include "Quaternion.h"
#include "VMath.h"

using namespace MATH;

//create a create quaternion out of the real and imaginary parts
Quaternion::Quaternion(float r, const Vec3& i)
{
	quat = i;
	quat.w = r;
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	quat.x = x;
	quat.y = y;
	quat.z = z;
	quat.w = w;
}

Quaternion::Quaternion(const Vec4& v)
{
	quat = v;
}

Quaternion::Quaternion()
{
	quat = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

//create a quaternion from an axis and degrees of rotation on that axis
Quaternion::Quaternion(const Vec3& axis, float degrees)
{
	degrees *= DEGREES_TO_RADIANS;
	quat = axis * sin(degrees / 2.0f);
	quat.w = cos(degrees / 2.0f);
}

float Quaternion::Mag() const
{
	const float mag = sqrt(quat.w * quat.w + quat.x * quat.x + quat.y * quat.y + quat.z * quat.z);
	
	return mag;
}

Quaternion Quaternion::Normalized() const
{
	return Quaternion(VMath::normalize(quat));
}

Quaternion Quaternion::Conjugate() const
{
	//invert the imaginary part of the quaternion
	//my vec4 class inherits from vec3 so I can pass quat into the (float, Vec3) constructor
	return Quaternion(quat.w, -quat);
}

Quaternion Quaternion::Inverse() const
{
	const float mag = Mag();
	//the inverse is the conjugate divided by Magnitude squared
	return Quaternion(Conjugate().quat / (mag * mag));
}

Vec3 Quaternion::Rotate(const Vec3& vec) const
{
	//create a quaternion with the real component of 0 and the imaginary component of the input vector
	const Quaternion p = Quaternion(0.0f, vec);
	//multiply by this quaternion then by our "Vector point" then by the conjugate of this quaternion
	//then return the Vec4 of that resulting quaternion
	return Vec3((*this * p * (*this).Conjugate()).quat);
}

Vec4 Quaternion::GetQuat() const
{
	return quat;
}

Matrix3 Quaternion::ConvertToMatrix() const
{
	Matrix3 m;

	m[0] = 1.0f - (2.0f * quat.y * quat.y) - (2.0f * quat.z * quat.z);
	m[1] = (2.0f * quat.x * quat.y) - (2.0f * quat.w * quat.z);
	m[2] = (2.0f * quat.x * quat.z) + (2.0f * quat.w * quat.y);
	m[3] = (2.0f * quat.x * quat.y) + (2.0f * quat.w * quat.z);
	m[4] = 1.0f - (2.0f * quat.x * quat.x) - (2.0f * quat.z * quat.z);
	m[5] = (2.0f * quat.y * quat.z) - (2.0f * quat.w * quat.x);
	m[6] = (2.0f * quat.x * quat.z) - (2.0f * quat.w * quat.y);
	m[7] = (2.0f * quat.y * quat.z) + (2.0f * quat.w * quat.x);
	m[8] = 1.0f - (2.0f * quat.x * quat.x) - (2.0f * quat.y * quat.y);
	return m;
}

Quaternion Quaternion::EulerToQuat(Vec3 v)
{
	Quaternion q;

	v *= DEGREES_TO_RADIANS;
	const float cosYaw = cos(v.z * 0.5f);
	const float sinYaw = sin(v.z * 0.5f);
	const float cosPit = cos(v.y * 0.5f);
	const float sinPit = sin(v.y * 0.5f);
	const float cosRol = cos(v.x * 0.5f);
	const float sinRol = sin(v.x * 0.5f);

	q.quat.w = cosRol * cosPit * cosYaw + sinRol * sinPit * sinYaw;
	q.quat.x = sinRol * cosPit * cosYaw - cosRol * sinPit * sinYaw;
	q.quat.y = cosRol * sinPit * cosYaw + sinRol * cosPit * sinYaw;
	q.quat.z = cosRol * cosPit * sinYaw - sinRol * sinPit * cosYaw;

	return q;
}


MATH::Vec3 Quaternion::GetVec3() const
{
	return Vec3(quat.x, quat.y, quat.z);
}

Vec3 MATH::Quaternion::QuatToEuler(Quaternion q)
{
	Vec3 e;

	const float sinPit_cosYaw = 2.0f * (q.quat.w * q.quat.x + q.quat.y * q.quat.z);
	const float cosPit_cosYaw = 1.0f - 2.0f * (q.quat.x * q.quat.x + q.quat.y * q.quat.y);

	e.x = atan2(sinPit_cosYaw, cosPit_cosYaw);

	const float sinYaw = 2.0f * (q.quat.w * q.quat.y - q.quat.z * q.quat.x);
	if (abs(sinYaw) >= 1.0f)
	{
		e.y = copysign(M_PI / 2.0f, sinYaw);
	}
	else
	{
		e.y = asin(sinYaw);
	}
	const float sinRol_cosYaw = 2.0f * (q.quat.w * q.quat.z + q.quat.x * q.quat.y);
	const float cosRol_cosYaw = 1.0f - 2.0f * (q.quat.y * q.quat.y + q.quat.z * q.quat.z);
	e.z = atan2(sinRol_cosYaw, cosRol_cosYaw);

	return e / DEGREES_TO_RADIANS;
}

Quaternion Quaternion::LookAt(const Quaternion& currentOrientation, const Vec3& at, const Vec3& up)
{
	const Vec3 direction = VMath::normalize(at);
	
	const Vec3 aiZAxis = currentOrientation.Rotate(up);

	//if its already facing
	if (aiZAxis == direction) {
		return currentOrientation;
	}
	else if(aiZAxis == direction * -1.0f)	{
		return currentOrientation * -1.0f;
	}

	//Otherwise face it
	Vec3 axis = VMath::cross(aiZAxis, direction);
	const float axisMag = VMath::mag(axis);
	const float angle = asin(axisMag);
	axis = VMath::normalize(axis);
	
	const float sinAngle = sin(angle / 2.0f);

	const Quaternion r(sinAngle * axis.x, sinAngle * axis.y, sinAngle * axis.z, cos(angle / 2.0f));
	const Quaternion b = currentOrientation.Inverse();
	
	return b * r;
}


Quaternion MATH::Quaternion::operator+=(const Vec3& v)
{
	quat.x += v.x;
	quat.y += v.y;
	quat.z += v.z;

	return *this;
}

Quaternion Quaternion::operator*(const float f) const
{
	return Quaternion(quat.x * f, quat.y * f, quat.z * f, quat.w * f);
}

Quaternion MATH::Quaternion::operator+=(const Quaternion& q) 
{
	quat.x += q.quat.x;
	quat.y += q.quat.y;
	quat.z += q.quat.z;
	quat.w += q.quat.w;

	return *this;
}
Quaternion MATH::Quaternion::operator+(const Quaternion& q) const
{
	return Quaternion(Vec4(quat.x + q.quat.x, quat.y + q.quat.y, quat.z + q.quat.z, quat.w + q.quat.w));
}


Quaternion Quaternion::operator+(const float f) const
{
	return Quaternion(Vec4(quat.x + f, quat.y + f, quat.z + f, quat.w + f));
}


Quaternion& Quaternion::operator=(const Vec3& q)
{
	quat.x = q.x;
	quat.y = q.y;
	quat.z = q.z;
	return *this;
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	//calculate the real component
	const float r = (quat.w * q.quat.w) - VMath::dot(quat, q.quat);
	//calculate the imaginary component
	const Vec3 i = (quat.w * q.quat) + (q.quat.w * quat) + VMath::cross(quat, q.quat);
	return Quaternion(r, i);
}

Quaternion Quaternion::operator*(const Vec4& v) const
{
	const float r = (quat.w * v.w) - VMath::dot(quat, v);
	const Vec3 i = (quat.w * v) + (v.w * quat) + VMath::cross(quat, v);

	return Quaternion(r, i);
}

Quaternion Quaternion::operator-() const
{
	Vec4 q = Vec4(-quat.x, -quat.y, -quat.z, -quat.w);
	return Quaternion(q);
}

std::ostream& MATH::operator<<(std::ostream& out, const Quaternion& q)
{
	out << "[" << q.GetQuat().w  << "(" << q.GetQuat().x << "," << q.GetQuat().y << "," << q.GetQuat().z << ")]";
	return out;
}
