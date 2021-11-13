#include "Edge.h"


#include "Node.h"
Edge::Edge(Node* ToNode, Node* FromNode, float Weight /*= 0.0f*/)
{
	N_ToNode = ToNode;
	N_FromNode = FromNode;
	F_Weight = Weight;
}

Edge::~Edge()
{
	N_ToNode = nullptr;
	N_FromNode = nullptr;
}
