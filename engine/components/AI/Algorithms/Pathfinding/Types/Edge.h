#ifndef EDGE_H
#define EDGE_H



class Node;
class Edge
{


public:
	Node* N_ToNode;
	Node* N_FromNode;
	float F_Weight;


	Edge(Node* ToNode, Node* FromNode= nullptr, float Weight = 0.0f);
	~Edge();


	inline const bool operator== (const Edge t2)
	{
		bool value = N_ToNode == t2.N_ToNode && N_FromNode == t2.N_FromNode;
		return value;
	}
};

#endif