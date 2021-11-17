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


//! AIDirector Class
/*! Manages all the information about the AI characters in our scene and what state the are currently in */
class AIDirector
{


private:

	//! PathAlgorithms Unordered_Map
	/*! Stores a pointer to the different pathing algorithms that can be used and allows us to look them up using an enum*/
	std::unordered_map<PathfindingAlgorithm,PathAlgorithm*> UM_PathAlgorithms;

	//! DivisionAlgorithms Unordered_Map
	/*! Stores a pointer to the different Division/Visibility Graph algorithms that can be used and allows us to look them up using an enum*/
	std::unordered_map<DivisionAlgorithm,BaseDivisionAlgo*> UM_DivisionAlgorithms;


	//! NavMesh Shared_Ptr
	/*! Will be utilized instead of the Visibility Graph Algorithms in the future to enhance efficiency*/
	std::shared_ptr<NavigationMesh> NavMesh;


	//! GetPositiveVerticies Getter
	/*! Looks through all the vertices's in the scene and compares there normals to check if they are pointing upwards within a certain threshold 
	and then stores them into a vector and returns them for us to use in our Division Scheme algorithms*/
	std::vector<Poly> GetPositiveVerticies() const;

	Graph* G_RecentlyGenerated;


	static std::unique_ptr<AIDirector> directorInstance;
	friend std::default_delete<AIDirector>;

public:

	AIDirector();
	~AIDirector();

	//! GetInstance Getter
	/*! Returns the singleton instance of this class*/
	static AIDirector* GetInstance();

	//! GenerateGraphFromMap Function
	/*! Will generate a graph representation of the map with whatever algorithm is passed in*/
	void GenerateGraphFromMap(DivisionAlgorithm Algorithm = DivisionAlgorithm::NAIVE);

	//! FindPath Function
	/*! Finds and returns the path to a given target with a given algorithm*/
	std::vector<MATH::Vec3> FindPath(Node* N_Target, Graph* traverableGraph = nullptr, PathfindingAlgorithm Algorithm = PathfindingAlgorithm::ASTAR);

	Graph* GetRecentlyGenerated() const;
};

#endif
