#ifndef OCTSPATIALPARTITION_H
#define OCTSPATIALPARTITION_H

#include <vector>

#include "math/Ray.h"
#include "core/MouseRay.h"


class Collider3D;
class GameObject;
class BoundingBox;

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
	void AddCollisionObject(Collider3D* collider);
	int GetObjectCount() const;
	bool IsLeaf() const;
	int GetChildCount() const;
	BoundingBox* GetBoundingBox() const;

private:
	BoundingBox* octBounds;

	OctNode* parent;
	OctNode* children[CHILDREN_NUMBER];
	std::vector<Collider3D*> objectList;
	float size;
	static int childNum;
	

	friend class OctSpatialPartition;
};

class OctSpatialPartition
{
public:
	OctSpatialPartition();
	OctSpatialPartition(float worldSize);
	~OctSpatialPartition();

	void AddObject(Collider3D* collider);
	GameObject* GetCollision(MouseRay& ray);

private:
	OctNode* root;
	std::vector<OctNode*> rayInstersectionList;
	void AddObjectToCell(OctNode* cell, Collider3D* collider);
	void PrepareCollisionQuery(OctNode* cell, MouseRay& ray);
	
};

#endif