#include "VMath.h"

using namespace MATH;
 
/// Return a normalized Vec3
Vec3 VMath::normalize(const Vec3 &a) {
	float magnitude;
	Vec3 result;
	magnitude = float(sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
#ifdef _DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
	if (magnitude < VERY_SMALL) {
		std::string errorMsg("Divide by nearly zero! ");
		throw errorMsg;
	}
#endif
	result.x = a.x / magnitude;
	result.y = a.y / magnitude;
	result.z = a.z / magnitude;
	return result;
}

Vec3 VMath::reflect(const Vec3 &v, const Vec3 &n){
	Vec3 result;
	float scalar = 2.0f * dot(-v, n);
	Vec3 temp = n * scalar;
	result = temp - v;
	return temp;
}

float VMath::distance(const Vec3 &a, const Vec3 &b){
	Vec3 r  = a - b;
	return(mag(r));
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
Vec3 MATH::VMath::orthagonalize(const Vec3 &v)
{
	try
	{
		if (abs(v.x) > abs(v.y))
		{
			if (abs(v.x) > abs(v.z))
			{
				return VMath::normalize(Vec3(v.x, 0.0f, 0.0f));
			}
			else
			{
				return VMath::normalize(Vec3(0.0f, 0.0f, v.z));
			}
		}
		else
		{
			if (abs(v.y) > abs(v.z))
			{
				return VMath::normalize(Vec3(0.0f, v.y, 0.0f));
			}
			else
			{
				return VMath::normalize(Vec3(0.0f, 0.0f, v.z));
			}
		}
	}
	catch (...)
	{

	}
}

Vec3 VMath::lerp(const Vec3 &v1, const Vec3 &v2, float t) {
	return (v1 + t * (v2 - v1));
}
