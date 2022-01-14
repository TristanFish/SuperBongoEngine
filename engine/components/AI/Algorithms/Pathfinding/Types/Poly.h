#ifndef POLY_H
#define POLY_H



#include <vector>

#include "Vector.h"
#include "Node.h"
class Poly
{
private:

	std::vector<MATH::Vec3> Verticies;

public:
	Poly() : Verticies(std::vector<MATH::Vec3>())
	{

	}


	~Poly()
	{

		if (!Verticies.empty())
		{
			Verticies.clear();
		}
	}

	inline void AddVertex(const MATH::Vec3& New_Vert)
	{
		Verticies.push_back(New_Vert);
	}
	

	inline std::vector<MATH::Vec3> GetVerticies() const {
		return Verticies;
	}

	inline std::vector<Node*> ConvertToNodes() const
	{
		std::vector<Node*> Nodes;
		for (int i = 0; i < Verticies.size(); i++)
		{
			Node* New_Node = new Node();
			New_Node->V_WorldPos = Verticies[i];
			Nodes.push_back(New_Node);
		}

		return Nodes;
	}

	inline bool IsEmpty() const { return Verticies.empty(); }

	inline int GetSize() const {
		return Verticies.size();
	}

};

#endif

