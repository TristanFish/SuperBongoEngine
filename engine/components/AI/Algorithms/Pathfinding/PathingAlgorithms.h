#ifndef PATHINGALGORITHMS_H
#define PATHINGALGORITHMS_H

#include <queue>
#include <set>
#include <map>
#include <functional>


#include "Types/Node.h"

class Graph;
namespace PathingAlgorithms
{

	class PathAlgorithm
	{

	protected:

		std::vector<Node*> V_CalculatedPath;

		Graph* G_CurrentGraph;


		Node* N_Goal;
	public:


		PathAlgorithm();


		virtual ~PathAlgorithm();

		virtual void CalculatePath() = 0;


		void SetCurrentGraph(Graph* newGraph);
		std::vector<Node*> GetCalculatedPath() const;
	};


	class BreadthFirst : public PathAlgorithm
	{

	private:

		std::queue<Node*> Q_Frontier;
		std::set<Node*> S_IteratedNodes;


	public:

		BreadthFirst();

		~BreadthFirst();

		virtual void CalculatePath() override;

	};


	class AStar : public PathAlgorithm
	{
	private:

		std::priority_queue<Node*,std::vector<Node*>, Node> PQ_Frontier;

		std::map<Node*, float> M_CostsToReach;


		float HeuristicFunc(Node* node);
	public:

		AStar();
		~AStar();


		virtual void CalculatePath() override;

	};
}



#endif

