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
		delete Algorithm.second;
		Algorithm.second = nullptr;
	}

}

void AIDirector::FindPath(Graph* traverableGraph, PathfindingAlgorithm Algorithm)
{
	UM_PathAlgorithms[Algorithm]->SetCurrentGraph(traverableGraph);
	UM_PathAlgorithms[Algorithm]->CalculatePath();
}
