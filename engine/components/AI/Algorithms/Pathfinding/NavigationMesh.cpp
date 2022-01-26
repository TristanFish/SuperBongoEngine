#include "NavigationMesh.h"

#include "core/Globals.h"

#include "components/3D/MeshRenderer.h"

NavigationMesh::NavigationMesh() : V_Polygons(std::vector<Poly>()), F_MaxWalkableAngle(45.0f)
{

}

NavigationMesh::~NavigationMesh()
{

}


void NavigationMesh::UpdateVerticies(const std::vector<Poly>& Polygons)
{
	V_Polygons = Polygons;
}

float NavigationMesh::GetMaxWalkableAngle() const
{
	return F_MaxWalkableAngle;
}

