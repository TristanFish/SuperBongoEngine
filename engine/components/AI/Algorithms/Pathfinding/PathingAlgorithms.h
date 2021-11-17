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

	//! PathAlgorithm Abstract class
	/*! Is used as a base for all our pathing algorithms and store's all of the basic information each algorithm uses*/
	class PathAlgorithm
	{

	protected:

		//! CalculatedPath vector
		/*! Stores the path that the algorithm has created for later use.*/
		std::vector<Node*> V_CalculatedPath;

		//! CurrentGraph Pointer
		/*! Stores a pointer to the current graph so it can be referenced throughout the algorithm*/
		Graph* G_CurrentGraph;

		//! N_Goal Pointer
		/*! Stores a pointer to the algorithms goal node*/
		Node* N_Goal;
	public:


		PathAlgorithm();


		virtual ~PathAlgorithm();

		//! CalculatePath Pure virtual function
		/*! Is the function that all other child classes will override to run their algorithm*/
		virtual void CalculatePath() = 0;

		//! GetCalculatedPath Getter
		/*! Returns the calculated path*/
		std::vector<Node*> GetCalculatedPath() const;
		
		//! SetGoal Setter
		/*! Set's the N_Goal variable so the algorithm can utilize it*/
		void SetGoal(Node* NGoal);

		//! SetCurrentGraph Setter
		/*! Set's the G_CurrentGraph variable so the algorithm can utilize it*/
		void SetCurrentGraph(Graph* newGraph);
		
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

