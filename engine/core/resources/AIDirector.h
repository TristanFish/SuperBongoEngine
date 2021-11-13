#ifndef AIDIRECTOR_H
#define AIDIRECTOR_H



#include "components/AI/Algorithms/Pathfinding/PathingAlgorithms.h"
#include "components/AI/Algorithms/Pathfinding/DivisionAlgorithms.h"
#include "components/AI/Algorithms/Pathfinding/NavigationMesh.h"


using namespace PathingAlgorithms;
using namespace DivisionAlgorithms;


enum class PathfindingAlgorithm
{
	ASTAR = 0,
	BREADTHFIRST = 1
};

enum class DivisionAlgorithm
{
	NAIVE = 0
};

class AIDirector
{


private:

	std::unordered_map<PathfindingAlgorithm,PathAlgorithm*> UM_PathAlgorithms;
	std::unordered_map<DivisionAlgorithm,BaseDivisionAlgo*> UM_DivisionAlgorithms;

	std::shared_ptr<NavigationMesh> NavMesh;


	std::vector<Poly> GetPositiveVerticies() const;



	static std::unique_ptr<AIDirector> directorInstance;
	friend std::default_delete<AIDirector>;

public:

	AIDirector();
	~AIDirector();

	static AIDirector* GetInstance();


	void GenerateGraphFromMap(DivisionAlgorithm Algorithm = DivisionAlgorithm::NAIVE);

	void FindPath(Graph* traverableGraph, PathfindingAlgorithm Algorithm = PathfindingAlgorithm::ASTAR);
};

#endif
