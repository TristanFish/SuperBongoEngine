#include "AIDirector.h"

AIDirector::AIDirector()
{
	UM_PathAlgorithms.emplace(PathfindingAlgorithm::ASTAR, new AStar());
	UM_PathAlgorithms.emplace(PathfindingAlgorithm::BREADTHFIRST, new BreadthFirst());




}

AIDirector::~AIDirector()
{

	for (auto Algorithm : UM_PathAlgorithms)
	{
		Algorithm.second = nullptr;
		delete Algorithm.second;
	}

}

void AIDirector::FindPath(Graph* traverableGraph, PathfindingAlgorithm Algorithm)
{
	UM_PathAlgorithms[Algorithm]->SetCurrentGraph(traverableGraph);
	UM_PathAlgorithms[Algorithm]->CalculatePath();
}
