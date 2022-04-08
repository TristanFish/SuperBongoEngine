#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H


#include "VMath.h"
#include "MMath.h"

#include "Collider3D.h"


class BoundingSphere : public Collider3D
{
private:

	Vec3 Min_Vertex, Max_Vertex;


	Vec3 Centre;
	float Radius;


public:

	inline BoundingSphere()
	{
		Min_Vertex = Max_Vertex = Vec3();

		Centre = Vec3();

		Radius = 0.1f;
	}
	inline BoundingSphere(const Vec3& New_Min, const Vec3& New_Max)
	{
		Min_Vertex = New_Min;
		Max_Vertex = New_Max;

		Centre = Vec3();

		Radius = 0.1f;
	}
	inline ~BoundingSphere()
	{

	}


	inline void CaclulateProperties(const Matrix4& transform)
	{
		Centre = (transform* Max_Vertex + transform* Min_Vertex) / 2.0f;

		for (const auto& vertex : *Model_Verticies)
		{
			Radius = std::max(Radius, VMath::mag(Centre - (transform *vertex.position)));
		}

		Radius -= 17;
	}


	inline const Vec3& GetCentre() { return Centre; }
	inline const float& GetRadius() { return Radius; }


};

#endif // BOUNDINGSPHERE_H
