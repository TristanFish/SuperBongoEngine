#include "Graph.h"

#include "Node.h"

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
	float Cost = 0.0f;

	for (Edge connect : current_Node->V_Connections)
	{
		if (connect.N_ToNode == next_Node)
		{
			Cost = connect.F_Weight;
		}

	}

	return Cost;
}

std::vector<Node*> Graph::GetNeighbours(Node* N_Node)
{
	std::vector<Node*> Neighbours;

	for (Edge connect : N_Node->V_Connections)
	{
		Neighbours.push_back(connect.N_ToNode);

	}

	return Neighbours;
}

Node* Graph::GetNode(MATH::Vec2 WorldPos) const
{
	for (auto node : V_Nodes)
	{
		if (MATH::Vec2(node->V_WorldPos.x, node->V_WorldPos.z) == WorldPos)
			return node;
	}

	return nullptr;
}

void Graph::AddNode(Node* N_Node)
{
	V_Nodes.push_back(N_Node);
}

void Graph::AddNodes(const std::vector<Node*>& V_NewNodes)
{
	for (auto node : V_NewNodes)
	{
		V_Nodes.push_back(node);
	}
}

Node* Graph::GetNode(int nodeIndex) const
{

	if ((nodeIndex) <= V_Nodes.size())
	{
		return V_Nodes[nodeIndex];
	}

	return nullptr;
}
