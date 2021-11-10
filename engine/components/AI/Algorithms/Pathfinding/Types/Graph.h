#ifndef GRAPH_H
#define GRAPH_H



#include <vector>
#include "Vector.h"


class Node;
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

