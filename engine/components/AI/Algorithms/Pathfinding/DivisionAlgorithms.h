#ifndef DIVISIONALGORITHMS_H
#define DIVISIONALGORITHMS_H


#include <vector>

#include "math/Vector.h"
#include "math/Ray.h"

class Graph;
namespace DivisionAlgorithms
{
	class BaseDivisionAlgo
	{
		protected:

		std::vector<MATH::Vec3> Verticies;

		Ray IntersectRay;

	public:

		BaseDivisionAlgo();
		virtual ~BaseDivisionAlgo();


		virtual Graph* ConstructDivision() = 0;

	};

	class NaiveVisAlgo : public BaseDivisionAlgo
	{
	private:



	public:

		NaiveVisAlgo();
		~NaiveVisAlgo();

		virtual Graph* ConstructDivision() override;

	};


}
#endif

