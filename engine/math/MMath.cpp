#include "VMath.h"
#include "MMath.h"

#include "core/Logger.h"
#include "Quaternion.h"

using namespace MATH;


Matrix4 MMath::calcRotationMatrix(const Vec3& euler)
{
	return	rotate(euler.x * DEGREES_TO_RADIANS, Vec3(1.0f, 0.0f, 0.0f)) *
			rotate(euler.y * DEGREES_TO_RADIANS, Vec3(0.0f, 1.0f, 0.0f)) *
		    rotate(euler.z * DEGREES_TO_RADIANS, Vec3(0.0f, 0.0f, 1.0f));
}

Vec3 MMath::calcEulerAngles(const Matrix4& rot)
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

Matrix4 MMath::GetRotationMat4(Vec3 forward, Vec3 up, Vec3 right)
{
	Matrix4 m;
	m[0] = right.x; m[1] = right.y;     m[2] = right.z; m[3] = 0;
	m[4] = up.x;    m[5] = up.y;        m[6] = up.z; m[7] = 0;
	m[8] = forward.x; m[9] = forward.y; m[10] = forward.z; m[11] = 0;
	m[12] = 0;      m[13] = 0;          m[14] = 0; m[15] = 1;

	return m;
}

Quaternion MMath::ConvertMatToQuat(const Matrix4& mat_)
{
	Quaternion q;
	float s;
	float x;
	float y;
	float z;
	float w;
	
	Matrix3 mat = mat_;
	float trace = mat[0] + mat[4] + mat[8];
	if(trace > 0)
	{
		s = sqrt(trace + 1.0f) * 2;
		w = 0.25f * s;
		x = (mat[7] - mat[5]) / s;
		y = (mat[2] - mat[6]) / s;
		z = (mat[3] - mat[1]) / s;
		return Quaternion(x, y, z, w);
	}
	else if((mat[0] > mat[4]) & (mat[0] > mat[8]))
	{
		s = sqrt(1.0f + mat[0] - mat[4] - mat[8]) * 2;
		w = (mat[7] - mat[5]) / s;
		x = 0.25f * s;
		y = (mat[1] + mat[3]) / s;
		z = (mat[2] + mat[6]) / s;
		return Quaternion(x, y, z, w);
	}
	else if(mat[4] > mat[8])
	{
		s = sqrt(1.0f + mat[4] - mat[0] - mat[8]) * 2;
		w = (mat[2] - mat[6]) / s;
		x = (mat[1] + mat[3]) / s;
		y = 0.25f * s;
		z = (mat[5] + mat[7]) / s;
		return Quaternion(x, y, z, w);
	}
	else
	{
		s = sqrt(1.0f + mat[8] - mat[0] - mat[4]) * 2;
		w = (mat[3] - mat[1]) / s;
		x = (mat[2] + mat[6]) / s;
		y = (mat[5] + mat[7]) / s;
		z = 0.25f * s;
		return Quaternion(x, y, z, w);
	}
}

MATH::Matrix4 MMath::GetFromMat3(const Matrix3& m_)
{
	Matrix4 m;

	m[0] = m_[0]; m[1] = m_[1]; m[2] = m_[2]; m[3] = 0;
	m[4] = m_[3]; m[5] = m_[4]; m[6] = m_[5]; m[7] = 0;
	m[8] = m_[6]; m[9] = m_[7]; m[10] = m_[8]; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;



	return m;
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

Matrix4 MMath::lookAt(const Vec3& eye, const Vec3& at, const Vec3& up){

	Matrix4 result;
	Vec3 modifiedUp = up;
	
	const Vec3 forward = VMath::normalize(at - eye);
	modifiedUp = VMath::normalize(modifiedUp);
	const Vec3 side = VMath::normalize( VMath::cross(forward,modifiedUp));
	modifiedUp = VMath::cross(side,forward);

	result[0] = side.x;
	result[1] = side.y;
	result[2] = side.z;
	result[3] = 0.0;

	result[4] = modifiedUp.x;
	result[5] = modifiedUp.y;
	result[6] = modifiedUp.z;
	result[7] = 0.0;

	result[8]  = -forward.x;
	result[9]  = -forward.y;
	result[10] = -forward.z;
	result[11] = 0.0;
	
	result[12] = -VMath::dot(side,eye);	
	result[13] = -VMath::dot(modifiedUp,eye);
	result[14] =  VMath::dot(forward,eye);
	result[15] = 1.0;

	return result;
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
		
		if ( fabs(determinate) < VERY_SMALL ) 
		{
			EngineLogger::Error("Divide by nearly zero error", "MMath.cpp", __LINE__, MessageTag::TYPE_MATH);
			determinate = 0;
		}
		else
		{
			determinate = 1.0f / determinate;
		}
		
		for (int i = 0; i < 16; i++){
			inverseM[i] *= determinate;
		}
		return inverseM;
}




bool MMath::DecomposeTransform(const Matrix4& transform, Vec3& outTransform, Quaternion& outQuaternion, Vec3& outScale)
{


	Matrix4 TransformMatrix(transform);

	if (epsilonEqual(TransformMatrix.getColumn(3)[3], 0.0f, 0.1f))
		return false;

	if (epsilonNotEqual(TransformMatrix.getColumn(0)[3], 0.0, 0.1) || epsilonNotEqual(TransformMatrix.getColumn(1)[3], 0.0, 0.1) || epsilonNotEqual(TransformMatrix.getColumn(2)[3], 0.0, 0.1))
	{
		TransformMatrix[12] = TransformMatrix[13] = TransformMatrix[14] = 0.0f;
		TransformMatrix[15] = 1.0f;
	}

	outTransform = MATH::Vec3(TransformMatrix.getColumn(3));
	TransformMatrix[12] = TransformMatrix[13] = TransformMatrix[14] = 0.0f;
	

	Vec3 Row[3], Pdum3;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Row[i][j] = TransformMatrix.getColumn(i)[j];
		}
	}

	outScale.x = VMath::mag(Row[0]);
	outScale.y = VMath::mag(Row[1]);
	outScale.z = VMath::mag(Row[2]);




	outQuaternion = ConvertMatToQuat(transform);

	return true;
}

bool MMath::epsilonEqual(const float& x, const float& y, const float& epsilon)
{
	return (x - y) < epsilon;
}

bool MMath::epsilonNotEqual(const float& x, const float& y, const float& epsilon)
{
	return (x - y) >= epsilon;

}
