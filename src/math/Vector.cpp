#include "Vector.h"

std::ostream& MATH::operator<<(std::ostream& out, const Vec3& v)
{
	out << "(" << v.x << ", " << v.y << ", " << v.z << ")";

	return out;
}
