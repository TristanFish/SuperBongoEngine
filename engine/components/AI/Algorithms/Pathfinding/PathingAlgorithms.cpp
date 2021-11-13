#include "PathingAlgorithms.h"

#include "Types/Graph.h"
using namespace PathingAlgorithms;

#pragma region Path Algorithm Base
PathAlgorithm::PathAlgorithm() : V_CalculatedPath(std::vector<Node*>()), G_CurrentGraph(nullptr)
{

}

PathAlgorithm::~PathAlgorithm()
{
	delete G_CurrentGraph;
	G_CurrentGraph = nullptr;
}


void PathAlgorithm::SetCurrentGraph(Graph* newGraph)
{
	G_CurrentGraph = newGraph;
}

std::vector<Node*> PathAlgorithm::GetCalculatedPath() const
{
	return V_CalculatedPath;
}

#pragma endregion 


#pragma region Breadth First Algorithm

BreadthFirst::BreadthFirst() : Q_Frontier(std::queue<Node*>()), S_IteratedNodes(std::set<Node*>())
{

}

BreadthFirst::~BreadthFirst()
{

}

void BreadthFirst::CalculatePath()
{
	Q_Frontier.push(G_CurrentGraph->GetNode(0));
	S_IteratedNodes.insert(G_CurrentGraph->GetNode(0));

	while (!Q_Frontier.empty())
	{
		// Initalize current node and pop the node on top of the queue
		Node* currentNode = Q_Frontier.front();
		Q_Frontier.pop();

		// Make sure current node is not equal to the goal
		if(currentNode == N_Goal)
			break;

		// Loop through each nodes connections
		for (auto connection : currentNode->V_Connections)
		{
			// Make sure that the connections to node has not been explored yet
			if (S_IteratedNodes.find(connection.N_ToNode) == S_IteratedNodes.end())
			{
				// insert the connections toNode and add current node to the final path
				Q_Frontier.push(connection.N_ToNode);
				S_IteratedNodes.insert(connection.N_ToNode);
				V_CalculatedPath.push_back(currentNode);
			}
		}
	}

	S_IteratedNodes.clear();

}

#pragma endregion


#pragma region AStar Algorithm

AStar::AStar() : PQ_Frontier(std::priority_queue<Node*, std::vector<Node*>, Node>())//, M_CostsToReach(std::map<Node*, float>())
{
	
}

AStar::~AStar()
{

}

void AStar::CalculatePath()
{
	PQ_Frontier.push(G_CurrentGraph->GetNode(0));
	M_CostsToReach.emplace(G_CurrentGraph->GetNode(0), 0.0f);

	
	
	while (!PQ_Frontier.empty())
	{
		Node* currentNode = PQ_Frontier.top();
		PQ_Frontier.pop();

		if(currentNode == N_Goal)
			break;


		for (auto nextNode : G_CurrentGraph->GetNeighbours(currentNode))
		{
			float newCost = M_CostsToReach[currentNode] + G_CurrentGraph->GetCost(currentNode, nextNode);

			if (M_CostsToReach.find(nextNode) == M_CostsToReach.end() || newCost < M_CostsToReach[nextNode])
			{
				M_CostsToReach.emplace(nextNode, newCost);
				
				float Priority = newCost + HeuristicFunc(nextNode);
				nextNode->F_Priority = Priority;
				PQ_Frontier.push(nextNode);

				V_CalculatedPath.push_back(nextNode);
			}
		}
	}
	
}

float AStar::HeuristicFunc(Node* node)
{
	float dx = abs(node->V_WorldPos.x - N_Goal->V_WorldPos.x);
	float dy = abs(node->V_WorldPos.y - N_Goal->V_WorldPos.y);

	return 1.0f * sqrt(dx * dx + dy * dy);
}

#pragma  endregion
