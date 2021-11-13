#include "DivisionAlgorithms.h"


#include "Types/Graph.h"
#include "Types/Node.h"
#include "Types/Edge.h"
#include "VMath.h"

using namespace DivisionAlgorithms;
#pragma region Base Division Algorithm

BaseDivisionAlgo::BaseDivisionAlgo() : V_Polygons(std::vector<Poly>()), R_IntersectRay(Ray())
{

}

BaseDivisionAlgo::~BaseDivisionAlgo()
{

}

void BaseDivisionAlgo::UpdatePolygons(const std::vector<Poly>& Polygons)
{
	V_Polygons = Polygons;
}

#pragma endregion


#pragma region Naive Division Algorithm


NaiveVisAlgo::NaiveVisAlgo()
{
	
}

NaiveVisAlgo::NaiveVisAlgo(std::vector<Poly> Polygons)
{
	V_Polygons = Polygons;
}

NaiveVisAlgo::~NaiveVisAlgo()
{

}

Graph* NaiveVisAlgo::ConstructDivision()
{
	Graph* newGraph = new Graph();

	std::vector<MATH::Vec3> WorldVerticies;

	for (int ni = 0; ni < V_Polygons.size(); ni++)
	{
		std::vector<Node*> Poly_Nodes = V_Polygons[ni].ConvertToNodes();

		for (auto node : Poly_Nodes)
			WorldVerticies.push_back(node->V_WorldPos);



		newGraph->AddNodes(Poly_Nodes);
	}

	for (auto poly : V_Polygons)
	{
		std::vector<MATH::Vec3> Verticies = poly.GetVerticies();
		for (int ni = 0; ni < Verticies.size(); ni++)
		{
			R_IntersectRay.origin = MATH::Vec3(Verticies[ni].x, Verticies[ni].z, 0.0f);

			for (int ng = 0; ng < WorldVerticies.size(); ng++)
			{

				if (WorldVerticies[ng] == Verticies[ni] || ng == WorldVerticies.size() - 1)
					continue;

				R_IntersectRay.end = MATH::Vec3(WorldVerticies[ng].x, WorldVerticies[ng].z, 0.0f);
				MATH::Vec2 v1 = MATH::Vec2(WorldVerticies[ng].x, WorldVerticies[ng].z);

				bool IntersectsBefore = false;
				for (int vert = 0; vert < WorldVerticies.size(); vert++)
				{
					if (vert == WorldVerticies.size() - 1)
					{
						break;
					}

					MATH::Vec2 p1 = MATH::Vec2(WorldVerticies[vert].x, WorldVerticies[vert].z);
					MATH::Vec2 p2 = MATH::Vec2(WorldVerticies[vert +1].x, WorldVerticies[vert +1].z);
					IntersectsBefore = R_IntersectRay.IntersectsLineSegment(p1, p2);



					if (IntersectsBefore)
						break;
				}

				if (!IntersectsBefore)
				{
					Node* N_ToNode = newGraph->GetNode(v1);
					MATH::Vec2 FromPos = MATH::Vec2(Verticies[ni].x, Verticies[ni].z);
					Node* N_FromNode = newGraph->GetNode(FromPos);


					N_FromNode->AddEdge(Edge(N_ToNode, N_FromNode));
				}

			}
		}

	}



	return newGraph;
}



#pragma endregion


