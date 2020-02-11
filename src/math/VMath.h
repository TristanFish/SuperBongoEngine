#ifndef VMATH_H
#define VMATH_H
#include <cmath>
#include <stdio.h>

/// This is a library of Vec3 math routines. 
#include "Vector.h"

namespace MATH {
	
	class VMath {

	public:
		/// Calculate the dot product between Vec3s a & b 
		inline static float dot(const Vec3 &a, const Vec3 &b){
			return(a.x * b.x + a.y * b.y + a.z * b.z);
		}

		
		/// Calulate the cross product
		inline static const Vec3 cross(const Vec3 &a, const Vec3 &b){
			return Vec3(a.y * b.z - a.z * b.y,
						a.z * b.x - a.x * b.z,
						a.x * b.y - a.y * b.x);
		}

		inline static float mag(const Vec3 &a) {
			return(sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
		}

		///  Angle-Axis rotation
		inline static Vec3 rotate(const Vec3 &n, float theta, const Vec3 &v) {
			return v * cos(theta) + VMath::dot(v, n) * n * (1.0f - cos(theta)) + VMath::cross(n, v) * sin(theta);
		}

		////////////////////////////////////////////////////////////////////////		
		/// These are a little more complex so I won't bother to inline them
		////////////////////////////////////////////////////////////////////////

		/// Return a normalized Vec3
		static Vec3 normalize(const Vec3 &a);
	
		/// Reflect off a normal  
		static Vec3 reflect(const Vec3 &v, const Vec3 &n);

		/// Get the distance between two Vec3s 
		static float distance(const Vec3 &a, const Vec3 &b);

		

		/// This is a basic Linear Interpolation function.  It takes v1 and moves it 
		/// to v2 in a linear fashion based on the value of t which goes from 
		/// 0.0 - 1.0.  This is a simple example of a parametric equation. The parameter
		/// the variable t can mean time but does not need be
		static Vec3 lerp(const Vec3 &v1, const Vec3 &v2, float t);
	};

}

#endif