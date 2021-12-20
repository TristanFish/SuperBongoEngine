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


	void AddNode(Node* N_Node);
	void AddNodes(const std::vector<Node*>& V_NewNodes);




	float GetCost(Node* current_Node, Node* next_Node);
	std::vector<Node*> GetNeighbours(Node* N_Node);

	Node* GetNode(MATH::Vec2 WorldPos) const;
	Node* GetNode(int nodeIndex) const;

};

#endif

