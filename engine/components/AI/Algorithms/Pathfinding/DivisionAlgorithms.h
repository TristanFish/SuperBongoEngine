#ifndef DIVISIONALGORITHMS_H
#define DIVISIONALGORITHMS_H


#include <vector>

#include "Types/Poly.h"
#include "math/Ray.h"

class Graph;
namespace DivisionAlgorithms
{
	class BaseDivisionAlgo
	{
	protected:
		std::vector<Poly> V_Polygons;
		Ray R_IntersectRay;

	public:

		BaseDivisionAlgo();
		virtual ~BaseDivisionAlgo();

		void UpdatePolygons(const std::vector<Poly>& Polygons);


		virtual Graph* ConstructDivision() = 0;

	};

	class NaiveVisAlgo : public BaseDivisionAlgo
	{
	private:
	

	public:

		NaiveVisAlgo();
		NaiveVisAlgo(std::vector<Poly> Verticies_);
		~NaiveVisAlgo();

		virtual Graph* ConstructDivision() override;

	};
}
#endif

