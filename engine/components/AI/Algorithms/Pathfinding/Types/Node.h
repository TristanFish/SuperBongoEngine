#ifndef NODE_H
#define NODE_H


#include <vector>

#include "Edge.h"
#include "math/Vector.h"
class Node
{
public:
	std::vector<Edge> V_Connections;

	float F_Priority;

	MATH::Vec3 V_WorldPos;

	Node()
	{
		V_Connections = std::vector<Edge>();

		F_Priority = 0.0f;
	}

	Node(const std::vector<Edge>& connections, float Priority = 0.0f)
	{
		V_Connections = connections;

		for (auto connect : connections)
			connect.N_FromNode = this;

		F_Priority = Priority;
	}

	~Node()
	{

	}

	inline void ConnectToNode(Node* toNode, float Weight = 0.0f)
	{
		V_Connections.push_back(Edge(toNode, this, Weight));
	}

	inline void AddEdge(Edge connection)
	{
		V_Connections.push_back(connection);

	}

	inline bool operator()(const Node* t1, const Node* t2)
	{
		return t1->F_Priority > t2->F_Priority;
	}
};

#endif