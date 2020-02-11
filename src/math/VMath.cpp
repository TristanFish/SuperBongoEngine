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
	float scalar = 2.0f * dot(v, n);
	Vec3 temp = n * scalar;
	result = temp - v;
	return result;
}

float VMath::distance(const Vec3 &a, const Vec3 &b){
	Vec3 r  = a - b;
	return(mag(r));
}

Vec3 VMath::lerp(const Vec3 &v1, const Vec3 &v2, float t) {
	return (v1 + t * (v2 - v1));
}
