#ifndef AIDIRECTOR_H
#define AIDIRECTOR_H



#include "components/AI/Algorithms/Pathfinding/PathingAlgorithms.h"
#include "components/AI/Algorithms/Pathfinding/DivisionAlgorithms.h"


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

	std::vector<MATH::Vec3> GetPositiveVerticies() const;

	static std::unique_ptr<AIDirector> directorInstance;
	friend std::default_delete<AIDirector>;

public:

	AIDirector();

	~AIDirector();

	static AIDirector* GetInstance();



	void FindPath(Graph* traverableGraph, PathfindingAlgorithm Algorithm = PathfindingAlgorithm::ASTAR);



};

#endif
