#include "GJKDetection.h"

#include "VMath.h"
#include "math/Matrix.h"
#include <cassert>

#include "core/Logger.h"

#pragma region 

Simplex::Simplex()	{
	simplexVertices.reserve(3);
}

Simplex::~Simplex()	{
	if(!simplexVertices.empty())	{
		simplexVertices.clear();
	}
}

Vec3 Simplex::Support(std::vector<Vec3> verticesVector1_, std::vector<Vec3> verticesVector2_, Vec3 direction_)	{
	//direction doesn't need to be normalized

	Vec3 point1 = getFarthestPointInDirection(verticesVector1_, direction_); //should be farthest point in direction_

	Vec3 point2 = getFarthestPointInDirection(verticesVector2_, -direction_); //should be farthest point in -direction_

	Vec3 MinkowskiPoint = point1 - point2;
	
	return MinkowskiPoint;
}

//code from comment by "l0k0" : https://www.gamedev.net/forums/topic/604497-farthest-point-in-a-direction/4825360/
Vec3 Simplex::getFarthestPointInDirection(std::vector<Vec3> verticesVector_ , Vec3 direction_)	{

	//mesh needs to have vertices, if not, error.
	assert(!verticesVector_.empty());

	//create a Vector containing all the positions of the meshes Vertices. As we only care about their pos
	//this Vector is only used in this function
	std::vector<Vec3> Vertices;
	for (auto& vertex : verticesVector_)	{
		Vertices.push_back(vertex);
	}
	
	int farthestIndex = 0;
	float farthestDistance = VMath::dot(Vertices.front(), direction_);
	float temp;

	//check distance of every point, and see if they are the farthest
	for (int i = 0; i < static_cast<int>(Vertices.size()); ++i) {
		temp = VMath::dot(Vertices.at(i), direction_);
		if(temp > farthestDistance)	{
			farthestDistance = temp;
			farthestIndex = i;
		}
	}
	
	
	//returns the farthest point found
	return Vertices.at(farthestIndex);
}

bool Simplex::containsOrigin(Vec3& direction_)	{

	Vec3 a = simplexVertices.front(); //Vec3 A

	Vec3 ao = Vec3(0.0f) - a; //A in the direction of 0

	if(simplexVertices.size() == 3)	{
		Vec3 b = simplexVertices.at(1);
		Vec3 c = simplexVertices.at(2);

		//the edges
		Vec3 ab = b - a;
		Vec3 ac = c - a;

		//triple cross product of ac x ab x ab
		//Vec3 abNormal = VMath::cross(ac, (VMath::cross(ab, ab)));
		Vec3 abNormal = VMath::cross(ab, ac);
		//triple cross product of ab x ac x ac
		Vec3 acNormal = -abNormal;

		Vec3 triangleCenter = Vec3((a.x + b.x + c.x) / 3.0f, (a.y + b.y + c.y) / 3.0f, (a.z + b.z + c.z) / 3.0f);

		//if origin is in the direction of abNormal
		if(VMath::dot(abNormal, triangleCenter) > 0.0f)	{
			//simplexVertices.pop_back(); // only delete if we are in 2D
			direction_ = abNormal;
		}
		else if (VMath::dot(acNormal, triangleCenter) > 0.0f) {		//now with acNormal
				//simplexVertices.erase(simplexVertices.begin() + 1); //delete b | only delete if we are in 2D
				direction_ = acNormal;
		}
		else	{ //is inside triangle
			
			direction_ = abNormal;
			

			
			
			return true;
			
		}
		
	}
	else if(simplexVertices.size() == 2)	{ //we have 2 points and want to make a triangle
		Vec3 b = simplexVertices.at(1);
		Vec3 ab = b - a;
		//triple cross product of ab x ao x ab
		Vec3 abNormal = VMath::cross(ab, (VMath::cross(ao, ab)));
		direction_ = abNormal;
	}
	else	{ //we have a tetrahedron - check if origin is inside
		Vec3 b = simplexVertices.at(1);
		Vec3 c = simplexVertices.at(2);
		Vec3 d = simplexVertices.at(3);

		Vec3 origin = Vec3(0.0f);

		//det0 is the "Decider", if a det is the same sign as sign0, then origin is inside that det, if its opposite then outside
		Matrix4 mat0 = Matrix4(a.x, a.y, a.z, 1.0f, b.x, b.y, b.z, 1.0f, c.x, c.y, c.z, 1.0f, d.x, d.y, d.z, 1.0f);
		float det0 = mat0.getDeterminant();
		bool sign0 = std::signbit(det0);

		Matrix4 mat1 = Matrix4(origin.x, origin.y, origin.z, 1.0f, b.x, b.y, b.z, 1.0f, c.x, c.y, c.z, 1.0f, d.x, d.y, d.z, 1.0f);
		float det1 = mat1.getDeterminant();
		bool sign1 = std::signbit(det1);

		Matrix4 mat2 = Matrix4(a.x, a.y, a.z, 1.0f, origin.x, origin.y, origin.z, 1.0f, c.x, c.y, c.z, 1.0f, d.x, d.y, d.z, 1.0f);
		float det2 = mat2.getDeterminant();
		bool sign2 = std::signbit(det2);
		
		Matrix4 mat3 = Matrix4(a.x, a.y, a.z, 1.0f, b.x, b.y, b.z, 1.0f, origin.x, origin.y, origin.z, 1.0f, d.x, d.y, d.z, 1.0f);
		float det3 = mat3.getDeterminant();
		bool sign3 = std::signbit(det3);
		
		Matrix4 mat4 = Matrix4(a.x, a.y, a.z, 1.0f, b.x, b.y, b.z, 1.0f, c.x, c.y, c.z, 1.0f, origin.x, origin.y, origin.z, 1.0f);
		float det4 = mat4.getDeterminant();
		bool sign4 = std::signbit(det4);

		if(sign0 == sign1 == sign2 == sign3 == sign4)	{
			//all sign match, so origin is inside the tetrahedron
			return true;
		}
		else	{
			if(sign1 != sign0)	{
				simplexVertices.erase(simplexVertices.begin());
				
			}
			if (sign2 != sign0) {
				simplexVertices.erase(simplexVertices.begin() + 1);
			}
			if (sign3 != sign0) {
				simplexVertices.erase(simplexVertices.begin() + 2);
			}
			if (sign4 != sign0) {
				simplexVertices.erase(simplexVertices.begin() + 3);

			}
		}
		
		
	}
	
	return false;
}

#pragma endregion //Simplex code



bool GJKDetection::GJKCollisionDetection(/*const Mesh& mesh1_, const Mesh& mesh2_*/std::vector<Vec3> verticesVector1_, std::vector<Vec3> verticesVector2_)	{

	if(verticesVector1_.empty() || verticesVector2_.empty())	{
		EngineLogger::Error("A mesh is missing its vertices in : ", "GJKDetection.cpp", __LINE__);
		
		return false;
	}
	
	
	Vec3 center1 = Vec3(0.0f);
	for (auto& vertex : verticesVector1_) {
		center1 += vertex;//pos
	}
	center1 /= verticesVector1_.size();

	Vec3 center2 = Vec3(0.0f);
	for (auto& vertex : verticesVector2_) {
		center2 += vertex;//pos
	}
	center2 /= verticesVector2_.size();

	//direction can be arbitrary, we chose  the distance in-between each objects center
	direction = center2 - center1; //this could be replaced with Origin2 - origin1 if we know origins of each mesh


	//first point
	simplex.simplexVertices.push_back(simplex.Support(verticesVector1_, verticesVector2_, direction));

	direction = -direction; //opposite direction for 2nd point

	while(true)	{
		//adds a new point to simplex. On 2nd iteration this uses -direction, on the 3rd it uses new direction from containsOrigin
		simplex.simplexVertices.push_back(simplex.Support(verticesVector1_, verticesVector2_, direction));

		if (VMath::dot(simplex.simplexVertices.back(), direction) <= 0.0f) {
			simplex.simplexVertices.clear();
			return false;
		}
		else if (simplex.containsOrigin(direction) && simplex.simplexVertices.size() == 4) { //this already checks if the simplex is big enough (4 points)
			simplex.simplexVertices.clear();
				return true;
		}
		
	}
}
