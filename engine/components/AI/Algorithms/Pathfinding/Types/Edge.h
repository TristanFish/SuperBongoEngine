#ifndef EDGE_H
#define EDGE_H



class Node;
class Connection
{


public:
	Node* N_ToNode;
	Node* N_FromNode;
	float F_Weight;


	Connection(Node* ToNode, Node* FromNode, float Weight = 0.0f);
	~Connection();
};

#endif