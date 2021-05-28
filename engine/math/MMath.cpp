#include "VMath.h"
#include "MMath.h"
using namespace MATH;


Matrix4 MATH::MMath::calcRotationMatrix(const Vec3& euler)
{
	return	MMath::rotate(euler.x * DEGREES_TO_RADIANS, Vec3(1.0f, 0.0f, 0.0f)) *
			MMath::rotate(euler.y * DEGREES_TO_RADIANS, Vec3(0.0f, 1.0f, 0.0f)) *
		    MMath::rotate(euler.z * DEGREES_TO_RADIANS, Vec3(0.0f, 0.0f, 1.0f));
}

Vec3 MATH::MMath::calcEulerAngles(const Matrix4& rot)
{
	Vec3 euler;

	Matrix4 rotMatrix = rot * DEGREES_TO_RADIANS;
	float botLeft = fabs(rotMatrix[2]);

	if (botLeft < 1.0f - VERY_SMALL || botLeft > 1.0f + VERY_SMALL)
	{
		euler.y = -asin(rotMatrix[2]);

		float cos1 = cos(euler.y);

#define PI_THRESH 0.01

		euler.x = atan2(rotMatrix[6] / cos1, rotMatrix[10] / cos1);
		if (fabs(euler.x) < M_PI + PI_THRESH && fabs(euler.x > M_PI - PI_THRESH))
		{
			euler.x = 0.0f;
		}

		euler.z = atan2(rotMatrix[1] / cos1, rotMatrix[0] / cos1);
		if (fabs(euler.z) < M_PI + PI_THRESH && fabs(euler.z > M_PI - PI_THRESH))
		{
			euler.z = 0.0f;
		}
#
	}
	else
	{
		euler.z = 0;

		if (rotMatrix[2] < 0)
		{
			euler.y = M_PI * 0.5f;
			euler.x = euler.z + atan2(rotMatrix[1], rotMatrix[2]);
		}
		else
		{
			euler.y = -M_PI * 0.5f;
			euler.x = -euler.z + atan2(-rotMatrix[1], -rotMatrix[2]);
		}
	}
	float rad2deg = 180.0f / 3.1415926f;
	return euler;// * rad2deg;
}

Matrix4 MMath::rotate(float degrees_, float x_, float y_, float z_){
	float cosang, sinang, cosm;
	Vec3 rotAxis(x_,y_,z_);
	rotAxis = VMath::normalize(rotAxis);
	//degrees_ *= DEGREES_TO_RADIANS;
	cosang = cos(degrees_);
	sinang = sin(degrees_);
	cosm = (1.0f - cosang);

	Matrix4 m;

	m[0] = (rotAxis.x * rotAxis.x * cosm) + cosang;
	m[1] = (rotAxis.x * rotAxis.y * cosm) + (rotAxis.z * sinang);
	m[2] = (rotAxis.x * rotAxis.z * cosm) - (rotAxis.y * sinang);
	m[3] = 0.0;
	m[4] = (rotAxis.x * rotAxis.y * cosm) - (rotAxis.z * sinang);
	m[5] = (rotAxis.y * rotAxis.y * cosm) + cosang;
	m[6] = (rotAxis.y * rotAxis.z * cosm) + (rotAxis.x * sinang);
	m[7] = 0.0;
	m[8] = (rotAxis.x * rotAxis.z * cosm) + (rotAxis.y * sinang);
	m[9] = (rotAxis.y * rotAxis.z * cosm) - (rotAxis.x * sinang);
	m[10] =(rotAxis.z * rotAxis.z * cosm) + cosang;
	m[11] = 0.0;
	m[12] = 0.0;	
	m[13] = 0.0;
	m[14] = 0.0;
	m[15] = 1.0;
	return m;
}

Matrix4 MMath::rotate(const float degrees_, const Vec3 &axis_) {
	 return MMath::rotate(degrees_, axis_.x, axis_.y, axis_.z);
}

/// As derived in GAME331 
Matrix4 MMath::perspective(const float fovy_, const float aspect_, const float zNear_, const float zFar_){
	float cot = 1.0f / tan(fovy_* 0.5f * DEGREES_TO_RADIANS);
	/// Don't forget, this looks row centric but it really is a column matrix - right-hand rule rules
	Matrix4 result(cot/aspect_, 0.0f,   0.0f,                             0.0f,
			      0.0f,		  cot,	  0.0f, 	                        0.0f,
			      0.0f,       0.0f,   (zNear_ + zFar_)/(zNear_ - zFar_),       -1.0,
			      0.0,        0.0,    (2.0f*zNear_*zFar_)/(zNear_-zFar_),   0.0);
	return result;
}


/// This creates a transform from Normalized Device Coordinates (NDC) to 
/// screen coordinates. OpenGL uses NDC as			 
///	              ------------------------------
///	             /|                           /|
///	            / |                          / |
///	           /  |                         /  |
///	          /   |                        /   |
///	         /    |                       /    |
///	        /     |                      /     |
///	       /      |                     /      |
///	      /       |                    /       |
///	     /        |                   /        |
///	    /         |                  /         |
///	   /----------------------------/ (1.0,1.0)|
///	   |          |                 |          |
///	   |          |                 |          |      +Y
///	   |          |                 |          | 
///	   |          |-----------------|----------|      ^
///	   |         /                  |         /       |
///	   |        /                   |        /        |       -Z
///	   |       /                    |       /         |
///	   |      /                     |      /          |     /
///	   |     /                      |     /           |    /
///	   |    /                       |    /            |   /
///	   |   /                        |   /             |  /
///	   |  /                         |  /              | /
///	   | /                          | /               |/
///	   |/ (-1.0,-1.0)               |/                ----------------> +X
///	   ------------------------------
Matrix4 MMath:: viewportNDC(int width_,int height_){
	float minZ = 0.0f;
	float maxZ = 1.0f;
	
	Matrix4 m;
	Matrix4 m1 = scale(1.0f, -1.0f, 1.0f);
	Matrix4 m2 = scale(float(width_)/2.0f, float(height_)/2.0f, maxZ - minZ);
	Matrix4 m3 = translate(float(width_)/2.0f,float(height_)/2.0f, minZ);
	m = m3 * m2 * m1;

	///This is the slightly faster way but who cares we do it rarely 
	/***
	m[0] = float(width_)/2.0f;
	m[5] = -float(height_)/2.0f; 
	m[10] =  maxZ - minZ;
	m[12] = float(width_)/2.0f;
	m[13] = float(height_)/2.0f;
	m[14] = minZ;
	m[15] = 1.0f; ***/

	return m;
}


/// 
Matrix4 MMath::orthographic(float xMin_, float xMax_, float yMin_, float yMax_, float zMin_, float zMax_){
	Matrix4 m;
	
	Matrix4 m1 = MMath::scale(2.0f / (xMax_ - xMin_), 2.0f / (yMax_ - yMin_),-2.0f / (zMax_ - zMin_));
	Matrix4 m2 = MMath::translate( -(xMax_ + xMin_) / (xMax_ - xMin_), -(yMax_ + yMin_) / (yMax_ - yMin_), -(zMax_ + zMin_) / (zMax_ - zMin_)); 
	m = m2 * m1;
	/*** Here's another way to do it
	m[0] = 2.0f / (xMax - xMin);
	m[5] = 2.0f / (yMax - yMin);
	m[10] = -2.0f / (zMax - zMin);
	m[12] = -((xMax + xMin) / (xMax - xMin));
	m[13] = -((yMax + yMin) / (yMax - yMin));
	m[14] = -((zMax + zMin) / (zMax - zMin));
	m[15] = 1.0f;
	***/
	return m;
}

/// The orthographic projection matrix is linear and affine but nothing else so there is has no inverse
/// Therefore, it is labeled singular or non-invertable.
/// I would still like to back transform from screen space to world space though
/// Here's my unOrtho - It undoes what the orthographic matrix creates
/// Multiply screen coordinates by this matrix and you should get x and y world coordinates
Matrix4 MMath::unOrtho(const Matrix4 & ortho){
	Matrix4 m;
	m[0] = 1.0f/ortho[0];
	m[5] = 1.0f/ortho[5];
	m[10] = 1.0f/ortho[10];
	m[12] = -ortho[12] * m[0];
	m[13] = -ortho[13] * m[5];
	m[14] = -ortho[14] * m[10];
	m[15] = 1.0f;
	return m;
}

/// At first glance, it might look like this matrix 
/// is written left-handed or transposed, it has not. 
/// Remember how memory is layed out. It is still column based.  
/// Tested Feb 1 2013 SSF  
Matrix4 MMath::translate(float x_, float y_, float z_){
	return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  x_,   y_,   z_,	1.0f);  
}
Matrix4 MMath::translate(const Vec3 &translate_) {
	return MMath::translate(translate_.x, translate_.y, translate_.z);
}

Matrix4 MMath::scale(float x_, float y_, float z_){
	return Matrix4(x_,  0.0f, 0.0f, 0.0f,
				  0.0f, y_ ,  0.0f, 0.0f,
				  0.0f, 0.0f, z_,   0.0f,
				  0.0f, 0.0f, 0.0f,	1.0f); 
}
Matrix4 MMath::scale(const Vec3 &scale) {
	return MMath::scale(scale.x, scale.y, scale.z);
}

///Tested Feb 1 2013 SSF
Matrix4 MMath::lookAt(float eyeX, float eyeY, float eyeZ,
			float atX, float atY, float atZ,
			float upX, float upY, float upZ){

	Vec3 at(atX,atY,atZ);
	Vec3 up(upX,upY,upZ);
	Vec3 eye(eyeX,eyeY,eyeZ);

	Matrix4 result;

	Vec3 forward = VMath::normalize(at - eye);
	up = VMath::normalize(up);
	Vec3 side = VMath::normalize( VMath::cross(forward,up));
	up = VMath::cross(side,forward);

	result[0] = side.x;
	result[1] = side.y;
	result[2] = side.z;
	result[3] = 0.0;

	result[4] = up.x;
	result[5] = up.y;
	result[6] = up.z;
	result[7] = 0.0;

	result[8]  = -forward.x;
	result[9]  = -forward.y;
	result[10] = -forward.z;
	result[11] = 0.0;
	
	result[12] = -VMath::dot(side,eye);	
	result[13] = -VMath::dot(up,eye);
	result[14] =  VMath::dot(forward,eye);
	result[15] = 1.0;

	return result;
}

Matrix4 MMath::lookAt(const Vec3& eye, const Vec3& at,  const Vec3& up){
	return lookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);
}

/// Take the transpose of a matrix, swap row with columns 
/// Tested 2016
Matrix4 MMath::transpose(const Matrix4 &m){
		return Matrix4(m[0], m[4], m[8], m[12],
					   m[1], m[5], m[9], m[13],
					   m[2], m[6], m[10],m[14],
					   m[3], m[7], m[11],m[15]);

}

/// 2x2 inverse is easy, 3x3 is a pain, 4x4 no way, this is tough stuff
/// Tested 2013
Matrix4 MMath::inverse(const Matrix4 &m) {
		Matrix4 inverseM;
		float determinate;

		inverseM[0] =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inverseM[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inverseM[2] =  m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inverseM[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inverseM[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inverseM[5] =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inverseM[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inverseM[7] =  m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inverseM[8] =  m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inverseM[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inverseM[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inverseM[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inverseM[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inverseM[13] =  m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inverseM[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inverseM[15] =  m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

		determinate = m[0] * inverseM[0] + m[1] * inverseM[4] + m[2] * inverseM[8] + m[3] * inverseM[12];
		
#ifdef _DEBUG  /// If in debug mode let's worry about divide by zero or nearly zero!!! 
		if ( fabs(determinate) < VERY_SMALL ) {
			std::string errorMsg("Divide by nearly zero in MMath::inverse!");
			throw errorMsg;
		}
#endif
		determinate = 1.0f / determinate;
		for (int i = 0; i < 16; i++){
			inverseM[i] *= determinate;
		}
		return inverseM;
}