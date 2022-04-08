#include "GJKDetection.h"

#include "VMath.h"
#include "math/Matrix.h"
#include <cassert>

#include "core/Logger.h"

#pragma region 

Simplex::Simplex() : simplexVertices({ Vec3(0), Vec3(0), Vec3(0), Vec3(0) }), m_size(0) {
	
}

Simplex::~Simplex()	{
	if(!simplexVertices.empty()) {
		
	}
}

Vec3 Simplex::Support(const std::vector<Vec3>& verticesVector1_, const std::vector<Vec3>& verticesVector2_, Vec3 direction_)	{
	//direction doesn't need to be normalized

	Vec3 point1 = getFarthestPointInDirection(verticesVector1_, direction_); //should be farthest point in direction_

	Vec3 point2 = getFarthestPointInDirection(verticesVector2_, -direction_); //should be farthest point in -direction_

	Vec3 MinkowskiPoint = point1 - point2;
	
	return MinkowskiPoint;
}

//code from comment by "l0k0" : https://www.gamedev.net/forums/topic/604497-farthest-point-in-a-direction/4825360/
Vec3 Simplex::getFarthestPointInDirection(const std::vector<Vec3>& verticesVector_ , Vec3 direction_)	{

	//mesh needs to have vertices, if not, error.
	assert(!verticesVector_.empty());
	
	Vec3 maxPoint;
	float maxDistance = -FLT_MAX;

	for (const Vec3& vertex : verticesVector_)
	{
		float distance = VMath::dot(vertex, direction_);

		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = vertex;
		}
	}

	return maxPoint;
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

		//testing ao instead of triangle center

		//if origin is in the direction of abNormal
		if(VMath::dot(abNormal, ao) > 0.0f)	{
			//simplexVertices.pop_back(); // only delete if we are in 2D
			direction_ = abNormal;
		}
		else if (VMath::dot(acNormal, ao) > 0.0f) {		//now with acNormal
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

		if(sign0 == sign1 && sign0 == sign2 && sign0 == sign3 && sign0 == sign4)	{
			//all sign match, so origin is inside the tetrahedron
			return true;
		}
		// if det0 == 0, then there is no tetrahedron, there is an issue. This is temp fix until we can find a better one
		else if(det0 == 0.0f)	{
			EngineLogger::Error("simplex is degenerate, there is an issue. in: ", "GJKDetection.cpp", __LINE__);
			return true;
		}
		//If any det is == 0.0f, then the plane is touching the point (so surface collision, no pen)
		else if(det1 == 0.0f || det2 == 0.0f || det3 == 0.0f || det4 == 0.0f)	{
			
			return true;
		}
		/*
		//one of the signs is different than Sign0, find the first one and try the whole thing again with a different point.
		else	{
			if(sign1 != sign0)	{
				simplexVertices.erase(simplexVertices.begin());
				return false;
			}
			if (sign2 != sign0) {
				simplexVertices.erase(simplexVertices.begin() + 1);
				return false;
			}
			if (sign3 != sign0) {
				simplexVertices.erase(simplexVertices.begin() + 2);
				return false;
			}
			if (sign4 != sign0) {
				simplexVertices.erase(simplexVertices.begin() + 3);
				return false;

			}
		}
		*/
		
	}
	
	return false;
}



#pragma endregion //Simplex code



void GJKDetection::AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b)
{

	auto reverse = std::find(edges.begin(), edges.end(), std::make_pair(faces[b], faces[a]));

	if (reverse != edges.end()) {
		edges.erase(reverse);
	}
	else
	{
		edges.emplace_back(faces[a], faces[b]);
	}
}

std::pair<std::vector<MATH::Vec4>, size_t> GJKDetection::GetFaceNormals(const std::vector<Vec3>& polytope, const std::vector<size_t>& faces)
{
	std::vector<Vec4> normals;
	size_t minTriangle = 0;
	float minDistance = FLT_MAX;

	for (size_t i = 0; i < faces.size(); i += 3)
	{
		Vec3 a = polytope[faces[i]];
		Vec3 b = polytope[faces[i+1]];
		Vec3 c = polytope[faces[i+2]];


		Vec3 normal = VMath::normalize(VMath::cross(b - a, c - a));

		float distance = VMath::dot(normal, a);

		if (distance < 0)
		{
			normal *= -1;
			distance *= -1;
		}

		normals.emplace_back(Vec4(normal, distance));

		if (distance < minDistance)
		{
			minTriangle = i / 3;
			minDistance = distance;
		}
	}

	return { normals,minTriangle };
}

GJKDetection::GJKDetection() : direction(Vec3())
{

}

GJKDetection::~GJKDetection()
{

}

CollisionHitInfo GJKDetection::EPA(Simplex& simplex, const std::vector<Vec3>& verticesVector1_, const std::vector<Vec3>& verticesVector2_)
{
	std::vector<Vec3> polytope(simplex.simplexVertices.begin(), simplex.simplexVertices.end());

	std::vector<size_t> faces = {
	0, 1, 2,
	0, 3, 1,
	0, 2, 3,
	1, 3, 2
	};

	auto [normals, minFace] = GetFaceNormals(polytope, faces);

	Vec3 minNormal;

	float minDistance = FLT_MAX;

	while (minDistance == FLT_MAX)
	{
		//Might Need to be changed to only get xyz
		minNormal = normals[minFace];

		minDistance = normals[minFace].w;

		Vec3 support = simplex.Support(verticesVector1_, verticesVector2_, minNormal);

		float sDistance = VMath::dot(minNormal, support);

		if (abs(sDistance - minDistance) > 0.001f)
		{
			minDistance = FLT_MAX;

			std::vector<std::pair<size_t, size_t>> uniqueEdges;

			for (size_t i = 0; i < normals.size(); i++)
			{				

				if (SameDirection(normals[i],support))
				{
					size_t f = i * 3;

					AddIfUniqueEdge(uniqueEdges, faces, f, f + 1);
					AddIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
					AddIfUniqueEdge(uniqueEdges, faces, f+2, f);

					faces[f + 2] = faces.back(); faces.pop_back();
					faces[f + 1] = faces.back(); faces.pop_back();
					faces[f    ] = faces.back(); faces.pop_back();
					
					normals[i] = normals.back(); normals.pop_back();


					i--;
				}
			}

			std::vector<size_t> newFaces;

			for (auto [edgeIndex1, edgeIndex2] : uniqueEdges)
			{
				newFaces.push_back(edgeIndex1);
				newFaces.push_back(edgeIndex2);
				newFaces.push_back(polytope.size());

			}
			polytope.push_back(support);

			auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);


			float oldMinDistance = FLT_MAX;
			
			for (size_t i = 0; i < normals.size(); i++)
			{
				if (normals[i].w < oldMinDistance)
				{
					oldMinDistance = normals[i].w;
					minFace = i;
				}
			}

			if (newNormals[newMinFace].w < oldMinDistance)
			{
				minFace = newMinFace + normals.size();
			}

			faces.insert(faces.end(), newFaces.begin(), newFaces.end());
			normals.insert(normals.end(), newNormals.begin(), newNormals.end());

		}
	}

	CollisionHitInfo HitInfo;

	HitInfo.Normal = minNormal;
	HitInfo.Depth = minDistance + 0.005f;
	HitInfo.HasCollision = true;


	return HitInfo;
}



bool GJKDetection::SameDirection(const Vec3& a, const Vec3& b)
{
	return MATH::VMath::dot(a, b) > 0.0f;
}

bool GJKDetection::Line(Simplex& points, Vec3& direction)
{
	Vec3 a = points.simplexVertices[0];
	Vec3 b = points.simplexVertices[1];

	Vec3 ab = b - a;
	Vec3 ao = -a;

	if (SameDirection(ab, ao))
	{
		direction = VMath::cross(VMath::cross(ab, ao), ab);
	}
	else
	{
		points = { a };
		direction = ao;
	}

	return false;
}

bool GJKDetection::Triangle(Simplex& points, Vec3& direction)
{
	Vec3 a = points.simplexVertices[0];
	Vec3 b = points.simplexVertices[1];
	Vec3 c = points.simplexVertices[2];

	Vec3 ab = b - a;
	Vec3 ac = c - a;
	Vec3 ao = -a;

	Vec3 abc = VMath::cross(ab, ac);

	if (SameDirection(VMath::cross(abc, ac), ao))
	{
		if (SameDirection(ac, ao))
		{
			points = { a,c };
			direction = VMath::cross(VMath::cross(ac, ao), ac);
		}
		else
		{
			return Line(points = { a,b }, direction);
		}
	}
	else
	{
		if (SameDirection(VMath::cross(ab, abc), ao))
		{
			return Line(points = { a,b }, direction);
		}
		else
		{
			if (SameDirection(abc, ao))
			{
				direction = abc;
			}
			else
			{
				points = { a,c,b };
				direction = -abc;
			}
		}

	}
	return false;
}

bool GJKDetection::Tetrahedron(Simplex& points, Vec3& direction)
{
	Vec3 a = points.simplexVertices[0];
	Vec3 b = points.simplexVertices[1];
	Vec3 c = points.simplexVertices[2];
	Vec3 d = points.simplexVertices[3];

	Vec3 ab = b - a;
	Vec3 ac = c - a;
	Vec3 ad = d - a;
	Vec3 ao = -a;

	Vec3 abc = VMath::cross(ab, ac);
	Vec3 acd = VMath::cross(ac, ad);
	Vec3 adb = VMath::cross(ad, ab);

	if (SameDirection(abc, ao))
	{
		return Triangle(points = { a, b, c }, direction);
	}
	if (SameDirection(acd, ao))
	{
		return Triangle(points = { a, c, d }, direction);
	}
	if (SameDirection(adb, ao))
	{
		return Triangle(points = { a, d, b }, direction);
	}
	return true;
}

bool GJKDetection::NextSimplex(Simplex& points, Vec3& direction)
{

	switch (points.size())
	{
	case 2: return Line(points, direction);
	case 3: return Triangle(points, direction);
	case 4: return Tetrahedron(points, direction);

	}


	return false;
}

bool GJKDetection::GJKCollisionDetection(const std::vector<Vec3>& verticesVector1_, const std::vector<Vec3>& verticesVector2_)	{

	if(verticesVector1_.size() < 4 || verticesVector2_.size() < 4)	{
		EngineLogger::Error("A mesh is missing its vertices, or has less than 4 points in : ", "GJKDetection.cpp", __LINE__);
		return false;
	}
	
	/*
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
	direction = center2 - center1; 
	
	//first point
	*/

	Simplex simplex;
	Vec3 support = simplex.Support(verticesVector1_, verticesVector2_, Vec3(1.0f,0.0f,0.0f));


	
	simplex.push_front(support);

	direction = -support; //opposite direction for 2nd point

	
	while(true)	{

		support = simplex.Support(verticesVector1_, verticesVector2_, direction);


		if (VMath::dot(support, direction) <= 0.0f) { // or  <= 0.0f
			// if the point added last was not past the origin in the direction of d
			// then the Minkowski Sum cannot possibly contain the origin since
			// the last point added is on the edge of the Minkowski Difference
			//simplex.simplexVertices.clear();
			return false;
		}

		simplex.push_front(support);

		if (NextSimplex(simplex, direction))
		{
			RecentHitInfo = EPA(simplex, verticesVector1_, verticesVector2_);
			return true;
		}


		/*
		//adds a new point to simplex. It uses new direction from containsOrigin
		simplex.simplexVertices.push_back(simplex.Support(verticesVector1_, verticesVector2_, direction));
		
		// make sure that the last point we added actually passed the origin
		if (VMath::dot(simplex.simplexVertices.back(), direction) <= VERY_SMALL) { // or  <= 0.0f
			// if the point added last was not past the origin in the direction of d
			// then the Minkowski Sum cannot possibly contain the origin since
			// the last point added is on the edge of the Minkowski Difference
			simplex.simplexVertices.clear();
			return false;
		}
		else if (simplex.containsOrigin(direction) && simplex.simplexVertices.size() == 4) { 

			RecentHitInfo = EPA(simplex, verticesVector1_, verticesVector2_);
			simplex.simplexVertices.clear();
				return true;
		}
		//there are times where it gets stuck in an infinite loop when it shoudnt be colliding, this prevents that.
		else if (direction == lastDirection) {
			//pretend like no collision
			simplex.simplexVertices.clear();
			return false;
		}
		lastDirection = direction;
		*/
	}

}
