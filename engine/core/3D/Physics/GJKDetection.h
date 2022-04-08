#ifndef GJKDETECTION_H
#define GJKDETECTION_H


#include "Vector.h"
#include "graphics/Mesh.h"


#include <array>
#include <algorithm>
using namespace  MATH;



struct CollisionHitInfo
{
	Vec3 Normal;
	float Depth;
	bool HasCollision;
};

//most GJK math will come from this site, but I have seen it a few other places too so I'm not sure if its original or not
//https://dyn4j.org/2010/04/gjk-gilbert-johnson-keerthi/#gjk-create
class Simplex	{
private:
	
	unsigned m_size;

public:
	std::array<Vec3,4> simplexVertices; //should be 4 Vec3s

	Simplex();
	~Simplex();
	
	Vec3 Support(const std::vector<Vec3>& verticesVector1_, const std::vector<Vec3>& verticesVector2_, Vec3 direction_); //Mesh should be replaced once we rework PHY


	
	//should be in the new 3d-phy-mesh class that will already have a mesh, so only arg would be direction_
	Vec3 getFarthestPointInDirection(const std::vector<Vec3>& verticesVector_, Vec3 direction_);

	//can change direction
	bool containsOrigin(Vec3& direction_);
	

	Simplex& operator=(std::initializer_list<Vec3> list) {

		for (auto v = list.begin(); v != list.end(); v++)
		{
			simplexVertices[std::distance(list.begin(), v)] = *v;
		}
		m_size = list.size();

		return *this;
	}

	void push_front(Vec3 point)
	{
		simplexVertices = { point, simplexVertices[0],simplexVertices[1] ,simplexVertices[2] };
		m_size = (std::min)(m_size + 1, 4u);
	}

	unsigned size() const { return m_size; }

};


class GJKDetection	{
private:
	Vec3 direction;

	CollisionHitInfo RecentHitInfo;


private:
	void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);

	std::pair<std::vector<Vec4>, size_t> GetFaceNormals(const std::vector<Vec3>& polytope, const std::vector<size_t>& faces);

public:

	GJKDetection();
	~GJKDetection();

	CollisionHitInfo EPA(Simplex& simplex,const std::vector<Vec3>& verticesVector1_, const std::vector<Vec3>& verticesVector2_);

	bool SameDirection(const Vec3& a, const Vec3& b);

	bool Line(Simplex& points, Vec3& direction);
	bool Triangle(Simplex& points, Vec3& direction);
	bool Tetrahedron(Simplex& points, Vec3& direction);

	bool NextSimplex(Simplex& points, Vec3& direction);

	bool GJKCollisionDetection(const std::vector<Vec3>& verticesVector1_, const std::vector<Vec3>& verticesVector2_);
	

	const CollisionHitInfo& GetRecentHitInfo() { return RecentHitInfo; }
};

#endif
