#ifndef NAVIGATIONMESH_H
#define NAVIGATIONMESH_H

#include <vector>

#include "math/Vector.h"
#include "math/Ray.h"

#include "Types/Poly.h"

class Graph;
class NavigationMesh
{
private:

	std::vector<Poly> V_Polygons;

	float F_MaxWalkableAngle;



	std::vector<Poly>& ConvertToConvex();

public:

	NavigationMesh();
	~NavigationMesh();

	void UpdateVerticies(const std::vector<Poly>& Polygons);


	float GetMaxWalkableAngle() const;

	Graph* ConstructGraph();


};

#endif
