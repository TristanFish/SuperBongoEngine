#ifndef COLLIDER3D_H
#define COLLIDER3D_H

//Not quite a component but similar enough to be in the same folder path
//Colliders define how rigidbodies will collide
struct Collider3D
{
public:

	enum class type : char
	{
		Sphere	 = 0b0001,
		AABB	 = 0b0010,
        Plane    = 0b0100,
		OBB		 = 0b1000
	};

	friend inline constexpr Collider3D::type operator | (Collider3D::type x, Collider3D::type y)
	{
		return static_cast<Collider3D::type>(static_cast<char>(x) | static_cast<char>(y));
	}

	type colliderType;
	bool isMoveable, isTrigger;

	MATH::Vec3 size;
	MATH::Vec3 minVertices,maxVertices;

	Collider3D(MATH::Vec3 minVert_,MATH::Vec3 maxVert_)
	{

		colliderType = type::AABB;
		isMoveable = true;
		isTrigger = false;
		size = MATH::Vec3(1.0f, 1.0f, 1.0f);
	}
};


#endif