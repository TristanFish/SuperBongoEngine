#include "DivisionAlgorithms.h"

using namespace DivisionAlgorithms;


#pragma region Base Division Algorithm

BaseDivisionAlgo::BaseDivisionAlgo() : Verticies(std::vector<MATH::Vec3>()), IntersectRay(Ray())
{

}

BaseDivisionAlgo::~BaseDivisionAlgo()
{

}

#pragma endregion


#pragma region Naive Division Algorithm


NaiveVisAlgo::NaiveVisAlgo()
{

}

NaiveVisAlgo::~NaiveVisAlgo()
{

}

Graph* NaiveVisAlgo::ConstructDivision()
{
	for (int ni = 0; ni < Verticies.size(); ni++)
	{
		for (const auto& ng : Verticies)
		{

		}


	}
}

#pragma endregion


