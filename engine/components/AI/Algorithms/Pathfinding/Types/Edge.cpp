#include "Edge.h"


#include "Node.h"
Connection::Connection(Node* ToNode, Node* FromNode, float Weight /*= 0.0f*/)
{
	N_ToNode = ToNode;
	N_FromNode = FromNode;
	F_Weight = Weight;
}

Connection::~Connection()
{
	N_ToNode = nullptr;
	N_FromNode = nullptr;
}
