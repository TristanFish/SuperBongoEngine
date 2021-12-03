#ifndef MMATH_H
#define MMATH_H
#include "Matrix.h"

///  A class devoted to graphics related Math - OpenGL style

namespace  MATH {

	struct Quaternion;
	
	class MMath {
	public:
		static Matrix4 calcRotationMatrix(const Vec3& euler);
		static Vec3 calcEulerAngles(const Matrix4& rotMatrix);

		static Matrix4 rotate(const float degrees_, const float x_, const float y_, const float z_);
		static Matrix4 rotate(const float degrees_, const Vec3 &axis_);

		static Matrix4 translate(const float x_, const float y_ , const float z_);
		static Matrix4 translate(const Vec3 &translate_);

		static Matrix4 scale(const float x_, const float y_, const float z_);
		static Matrix4 scale(const Vec3 &scale);

		static Matrix4 perspective(const float fovy_, const float aspect_, const float zNear_, const float zFar_);
		static Matrix4 viewportNDC(const int width_, const int height_);
		static Matrix4 orthographic(const float xMin_, const float xMax_,
									const float yMin_, const float yMax_,
									const float zMin_, const float zMax_);
		static Matrix4 unOrtho(const Matrix4 & ortho); 
		static Matrix4 lookAt(const Vec3& eye, const Vec3& at, const Vec3& up);					
		static Matrix4 transpose(const Matrix4 &m);
		static Matrix4 inverse(const Matrix4 &m);

		static Matrix4 GetFromMat3(const Matrix3& m_);
		static Matrix4 GetRotationMat4(Vec3 forward, Vec3 up, Vec3 right);
		static Quaternion ConvertMatToQuat(const Matrix4& mat);

	};

}
#endif