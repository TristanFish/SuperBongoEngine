#ifndef GRAPH_H
#define GRAPH_H



#include <vector>
#include "Vector.h"


class Node;
struct Connection
{

	
public:
	Node* N_ToNode;
	Node* N_FromNode;
	float F_Weight;


	Connection(Node* ToNode,  Node* FromNode, float Weight = 0.0f)
	{
		N_ToNode = ToNode;
		N_FromNode = FromNode;
		F_Weight = Weight;
	}

	~Connection()
	{
		N_ToNode = nullptr;
		N_FromNode = nullptr;
	}
};


class Graph
{

private:

	std::vector<Node*> V_Nodes;


public:

	Graph();
	Graph(std::vector<Node*> Nodes);
	~Graph();


	float GetCost(Node* current_Node, Node* next_Node);
	std::vector<Node*> GetNeighbours(Node* N_Node);

	Node* GetNode(int nodeIndex) const;

};

#endif

