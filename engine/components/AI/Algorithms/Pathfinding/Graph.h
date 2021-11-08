#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Vector.h"
struct Connection
{
	Node* N_ToNode;
	Node* N_FromNode;
	float F_Weight;


	Connection(Node* ToNode, Node* FromNode, float Weight = 0.0f)
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

struct Node
{

	std::vector<Connection> V_Connections;

	float F_Priority;

	MATH::Vec3 V_WorldPos;



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

