#ifndef GJKDETECTION_H
#define GJKDETECTION_H


#include "Vector.h"
#include "graphics/Mesh.h"

using namespace  MATH;


//most GJK math will come from this site, but I have seen it a few other places too so I'm not sure if its original or not
//https://dyn4j.org/2010/04/gjk-gilbert-johnson-keerthi/#gjk-create
class Simplex	{
private:
	

public:
	std::vector<Vec3> simplexVertices; //should be 4 Vec3s
	
	Simplex();
	~Simplex();
	
	Vec3 Support(std::vector<Vec3> verticesVector1_, std::vector<Vec3> verticesVector2_, Vec3 direction_); //Mesh should be replaced once we rework PHY

	
	//should be in the new 3d-phy-mesh class that will already have a mesh, so only arg would be direction_
	Vec3 getFarthestPointInDirection(std::vector<Vec3> verticesVector_, Vec3 direction_);

	//can change direction
	bool containsOrigin(Vec3& direction_);
	
};


class GJKDetection	{
private:
	Simplex simplex;
	Vec3 direction;

public:

	bool GJKCollisionDetection(std::vector<Vec3> verticesVector1_, std::vector<Vec3> verticesVector2_);
	
};

#endif
