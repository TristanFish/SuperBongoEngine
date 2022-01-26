#include "VMath.h"

#include "core/Logger.h"

using namespace MATH;
 
/// Return a normalized Vec3
Vec3 VMath::normalize(const Vec3 &a) {
	Vec3 result;

	const float magnitude = static_cast<float>(sqrt(a.x * a.x + a.y * a.y + a.z * a.z));

	if (magnitude < VERY_SMALL) {
		EngineLogger::Warning("Divide by nearly zero", "VMATH.cpp", __LINE__, MessageTag::TYPE_MATH);
		result = Vec3();
	}
	else
	{
		result = a / magnitude;
	}

	return result;
}

Vec3 VMath::reflect(const Vec3 &v, const Vec3 &n)
{
	const float scalar = 2.0f * dot(-v, n);
	const Vec3 temp = n * scalar;
	const Vec3 result = temp - v;
	return result;
}

float VMath::distance(const Vec3 &a, const Vec3 &b)
{
	return mag(a - b);
}

Vec4 VMath::normalize(const Vec4& a)
{
	Vec4 result;
	const float magnitude = static_cast<float>(sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w));
	if (magnitude < VERY_SMALL) 
	{
		EngineLogger::Warning("Divide by nearly zero!", "VMATH.cpp", __LINE__, MessageTag::TYPE_MATH);
		result = Vec4();
	}
	else
	{
		result = a / magnitude;
	}

	return result;
}

MATH::Vec2 VMath::normalize(const Vec2& a)
{
	float magnitude;
	Vec2 result;
	magnitude = float(sqrt(a.x * a.x + a.y * a.y));
#ifdef _DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
	if (magnitude < VERY_SMALL) {
		std::string errorMsg("Divide by nearly zero! ");
		throw errorMsg;
	}
#endif
	result.x = a.x / magnitude;
	result.y = a.y / magnitude;
	return result;
}

Vec3 MATH::VMath::clamp(const Vec3& clampedV, const Vec3& minV, const Vec3& maxV)
{
	Vec3 clampedVector = clampedV;

	//Check if any components of the clampedVector are less than the min value,
	//if they are increase the value to the minimum value
	if (clampedVector.x < minV.x)
	{
		clampedVector.x = minV.x;
	}
	if (clampedVector.y < minV.y)
	{
		clampedVector.y = minV.y;
	}
	if (clampedVector.z < minV.z)
	{
		clampedVector.z = minV.z;
	}

	//Check if any components of the clampedVector are greater than the max value,
	//if they are decrease the value to the maximum value
	if (clampedVector.x > maxV.x)
	{
		clampedVector.x = maxV.x;
	}
	if (clampedVector.y > maxV.y)
	{
		clampedVector.y = maxV.y;
	}
	if (clampedVector.z > maxV.z)
	{
		clampedVector.z = maxV.z;
	}

	return clampedVector;
}

//returns a vector where the largest component of the input vector is now a unit length
//zeroes out all other components
Vec3 VMath::orthagonalize(const Vec3 &v)
{
	if (abs(v.x) > abs(v.y))
	{
		if (abs(v.x) > abs(v.z))
		{
			if(v.x > 0)
			{
				return Vec3::Right();
			}
			else
			{
				return -Vec3::Right();
			}
		}
		else
		{
			if(v.z > 0)
			{
				return Vec3::Forward();
			}
			else
			{
				return -Vec3::Forward();
			}
		}
	}
	else
	{
		if (abs(v.y) > abs(v.z))
		{
			if(v.y > 0)
			{
				return Vec3::Up();
			}
			else
			{
				return -Vec3::Up();
			}
		}
		else
		{
			if(v.z > 0)
			{
				return Vec3::Forward();
			}
			else
			{
				return -Vec3::Forward();
			}
		}
	}
}

Vec3 VMath::lerp(const Vec3 &v1, const Vec3 &v2, float t) {
	return (v1 + t * (v2 - v1));
}