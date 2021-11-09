#ifndef NODE_H
#define NODE_H

#include <vector>

#include "Graph.h"
#include "math/Vector.h"

struct Node
{

	std::vector<Connection> V_Connections;

	float F_Priority;

	MATH::Vec3 V_WorldPos;

	Node()
	{
		V_Connections = std::vector<Connection>();

		F_Priority = 0.0f;
	}

	Node(std::vector<Connection>& connections, float Priority = 0.0f)
	{
		V_Connections = connections;

		F_Priority = Priority;
	}

	~Node()
	{

	}

	inline void ConnectToNode(Node* toNode, float Weight = 0.0f)
	{
		V_Connections.push_back(Connection(toNode, this, Weight));
	}

	inline void AddConnection(Connection connection)
	{
		V_Connections.push_back(connection);
	}

	inline bool operator()(const Node& t1, const Node& t2)
	{
		return t1.F_Priority > t2.F_Priority;
	}
};

#endif