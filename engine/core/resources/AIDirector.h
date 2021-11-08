#ifndef AIDIRECTOR_H
#define AIDIRECTOR_H



#include "components/AI/Algorithms/Pathfinding/PathingAlgorithms.h"


using namespace PathingAlgorithms;

enum class PathfindingAlgorithm
{
	ASTAR = 0,
	BREADTHFIRST = 1
};

class AIDirector
{


private:

	std::unordered_map<PathfindingAlgorithm,PathAlgorithm*> UM_PathAlgorithms;



public:

	AIDirector();

	~AIDirector();


	void FindPath(Graph* traverableGraph, PathfindingAlgorithm Algorithm = PathfindingAlgorithm::ASTAR);



};

#endif
