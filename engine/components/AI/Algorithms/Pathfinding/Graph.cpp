#include "Graph.h"

Graph::Graph() : V_Nodes(std::vector<Node*>())
{

}

Graph::Graph(std::vector<Node*> Nodes)
{
	V_Nodes = Nodes;
}

Graph::~Graph()
{

	for (Node* node : V_Nodes)
	{
		node = nullptr;
		delete node;
	}

}



float Graph::GetCost(Node* current_Node, Node* next_Node)
{
	float Cost;

	for (Connection connect : current_Node->V_Connections)
	{
		if (connect.N_ToNode == next_Node)
		{
			Cost = connect.F_Weight;
		}

	}
}

std::vector<Node*> Graph::GetNeighbours(Node* N_Node)
{
	std::vector<Node*> Neighbours;

	for (Connection connect : N_Node->V_Connections)
	{
		Neighbours.push_back(connect.N_ToNode);

	}

	return Neighbours;
}

Node* Graph::GetNode(int nodeIndex) const
{

	if ((nodeIndex) <= V_Nodes.size())
	{
		return V_Nodes[nodeIndex];
	}

	return nullptr;
}
