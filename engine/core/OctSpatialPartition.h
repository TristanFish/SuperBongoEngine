#ifndef OCTSPATIALPARTITION_H
#define OCTSPATIALPARTITION_H

#include <vector>

#include "math/Ray.h"
#include "custom/MouseRay.h"



struct OrientedBoundingBox;
class MeshRenderer;
class GameObject;

constexpr auto CHILDREN_NUMBER = 8;

enum class OctChildren
{
	OCT_TLF,
	OCT_BLF,
	OCT_TRF,
	OCT_BRF,
	OCT_TLR,
	OCT_BLR,
	OCT_TRR,
	OCT_BRR,
};


class OctNode
{
public:
	OctNode(MATH::Vec3 position, float size, OctNode* parent);
	~OctNode();

	void Octify(int depth);
	OctNode* GetParent() const;
	OctNode* GetChild(OctChildren childPos);
	void AddCollisionObject(MeshRenderer* rb);
	int GetObjectCount() const;
	bool IsLeaf() const;
	int GetChildCount() const;
	OrientedBoundingBox* GetBoundingBox() const;

private:
	friend class OctSpatialPartition;
	OrientedBoundingBox* octBounds;
	OctNode* parent;
	OctNode* children[CHILDREN_NUMBER];
	std::vector<MeshRenderer*> objectList;
	float size;
	static int childNum;
	
};

class OctSpatialPartition
{
public:
	OctSpatialPartition();
	OctSpatialPartition(float worldSize);
	~OctSpatialPartition();

	void AddObject(MeshRenderer* rb);
	GameObject* GetCollision(MouseRay& ray);

private:
	OctNode* root;
	std::vector<OctNode*> rayInstersectionList;
	void AddObjectToCell(OctNode* cell, MeshRenderer* rb);
	void PrepareCollisionQuery(OctNode* cell, MouseRay& ray);
	
};

#endif