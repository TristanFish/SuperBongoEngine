#include "DivisionAlgorithms.h"


#include "Types/Graph.h"
#include "Types/Node.h"
#include "Types/Edge.h"
#include "VMath.h"

#include <map>

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

	// 
	std::vector<MATH::Vec3> WorldVerticies;
	std::vector<int> vertexTypeID;

	for (size_t ni = 0; ni < V_Polygons.size(); ni++)
	{
		std::vector<Node*> Poly_Nodes = V_Polygons[ni].ConvertToNodes();

		for (auto node : Poly_Nodes)
		{
			WorldVerticies.push_back(node->V_WorldPos);
			vertexTypeID.push_back(ni);
		}



		newGraph->AddNodes(Poly_Nodes);
	}

	for (auto poly : V_Polygons)
	{
		std::vector<MATH::Vec3> Verticies = poly.GetVerticies();
		for (size_t ni = 0; ni < Verticies.size(); ni++)
		{
			R_IntersectRay.origin = MATH::Vec3(Verticies[ni].x, Verticies[ni].z, 0.0f);

			for (size_t ng = 0; ng < WorldVerticies.size(); ng++)
			{

				if (WorldVerticies[ng] == Verticies[ni])
					continue;

				R_IntersectRay.end = MATH::Vec3(WorldVerticies[ng].x, WorldVerticies[ng].z, 0.0f);

				bool IntersectsBefore = false;
				int firstOfType = 0;
				// Loops through all verticies again and makes sure the ray doesn't intersect their line segments
				for (size_t vert = 0; vert < WorldVerticies.size(); vert++)
				{

					if (WorldVerticies[vert] == WorldVerticies[ng])
					{
						continue;
					}
					if(vert == WorldVerticies.size() - 1)
						break;

					int nextTypeIndex = vert + 1;

					/* Make's sure that if we are at the end of a certain shape that it 
					will set the next index to the first of that shape to make sure we fully loop around it.*/ 
					if(vertexTypeID[vert + 1] != vertexTypeID[vert])
					{
						nextTypeIndex = firstOfType;

						firstOfType = vert + 1;
					}

				
					MATH::Vec2 p1 = MATH::Vec2(WorldVerticies[vert].x, WorldVerticies[vert].z);
					MATH::Vec2 p2 = MATH::Vec2(WorldVerticies[nextTypeIndex].x, WorldVerticies[nextTypeIndex].z);
					IntersectsBefore = R_IntersectRay.IntersectsLineSegment(p1, p2);

					
					if (IntersectsBefore)
						break;
				}


				// Add's a new edge to the from node with popper weight
				if (!IntersectsBefore)
				{
					MATH::Vec2 ToPos = MATH::Vec2(WorldVerticies[ng].x, WorldVerticies[ng].z);


					Node* N_ToNode = newGraph->GetNode(ToPos);
					MATH::Vec2 FromPos = MATH::Vec2(Verticies[ni].x, Verticies[ni].z);

					Node* N_FromNode = newGraph->GetNode(FromPos);

					float Edge_Weight = MATH::VMath::mag(FromPos - ToPos);

					Edge E_NewEdge = Edge(N_ToNode, N_FromNode);
					E_NewEdge.F_Weight = Edge_Weight;

					N_FromNode->AddEdge(E_NewEdge);
				}

			}
		}

	}



	return newGraph;
}



#pragma endregion


