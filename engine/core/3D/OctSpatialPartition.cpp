#include "OctSpatialPartition.h"

#include "core/MouseRay.h"
#include "core/3D/Physics3D.h"
#include "components/ECS.h"
#include "core/Logger.h"

int OctNode::childNum = 0;

using namespace MATH;

OctNode::OctNode(Vec3 position, float size, OctNode* parent) 
{
	this->size = size;

	octBounds = new OrientedBoundingBox();
	octBounds->minVert = position;
	octBounds->maxVert = position + Vec3(size);

	this->parent = parent;

	for(int i = 0; i < CHILDREN_NUMBER; i++)
	{
		children[i] = nullptr;
	}
}

OctNode::~OctNode()
{
	delete octBounds;
	octBounds = nullptr;

	if(objectList.size() > 0)
	{
		for(auto obj : objectList)
		{
			obj = nullptr;
		}
	}

	for(int i = 0; i < CHILDREN_NUMBER; i++)
	{
		if(children[i] != nullptr)
		{
			delete children[i];
		}
		children[i] = nullptr;
	}
}

void OctNode::Octify(int depth)
{
	if(depth > 0 && this)
	{
		float half = size / 2.0f;
		children[static_cast<int>(OctChildren::OCT_TLF)] = 
			new OctNode(Vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_BLF)] = 
			new OctNode(Vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_TRF)] = 
			new OctNode(Vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_BRF)] = 
			new OctNode(Vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z + half), half, this);

		children[static_cast<int>(OctChildren::OCT_TLR)] = 
			new OctNode(Vec3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z), half, this);

		children[static_cast<int>(OctChildren::OCT_BLR)] = 
			new OctNode(Vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z), half, this);

		children[static_cast<int>(OctChildren::OCT_TRR)] = 
			new OctNode(Vec3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z), half, this);

		children[static_cast<int>(OctChildren::OCT_BRR)] = 
			new OctNode(Vec3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z), half, this);

		
		
		childNum += 8;
	}

	if(depth > 0 && this)
	{
		for(int i = 0; i < CHILDREN_NUMBER; i++)
		{
			children[i]->Octify(depth - 1);
		}
	}
}

OctNode* OctNode::GetParent() const
{
	return parent;
}

OctNode* OctNode::GetChild(OctChildren childPos)
{
	return children[static_cast<int>(childPos)];
}

void OctNode::AddCollisionObject(MeshRenderer* rb)
{
	objectList.push_back(rb);
}

int OctNode::GetObjectCount() const
{
	return objectList.size();
}

bool OctNode::IsLeaf() const
{
	return children[0];
}

int OctNode::GetChildCount() const
{
	if(IsLeaf())
	{
		return 0;
	}
	return CHILDREN_NUMBER;
}

OrientedBoundingBox* OctNode::GetBoundingBox() const
{
	return octBounds;
}

OctSpatialPartition::OctSpatialPartition()
{
	
}

OctSpatialPartition::OctSpatialPartition(float worldSize)
{
	root = new OctNode(Vec3(-(worldSize / 2.0f)), worldSize, nullptr);
	root->Octify(3);

	EngineLogger::Info("ChildNodes: " + std::to_string(root->GetChildCount()), "OctSpatialPartition.cpp", __LINE__);

	rayInstersectionList.reserve(20);
}

OctSpatialPartition::~OctSpatialPartition()
{
	if(rayInstersectionList.empty())
	{
		for(auto cell : rayInstersectionList)
		{
			cell = nullptr;
		}

		rayInstersectionList.clear();
	}

	delete root;
	root = nullptr;
}

void OctSpatialPartition::AddObject(MeshRenderer* rb)
{
	AddObjectToCell(root, rb);
}

GameObject* OctSpatialPartition::GetCollision(MouseRay& ray)
{
	if(rayInstersectionList.size() > 0)
	{
		for(auto cell : rayInstersectionList)
		{
			cell = nullptr;
		}
		rayInstersectionList.clear();
	}
	PrepareCollisionQuery(root, ray);

	GameObject* hitResult = nullptr;
	float shortestDistance = FLT_MAX;
	
	for (auto* cell : rayInstersectionList)
	{
		for(auto obj : cell->objectList)
		{
			
			if (Physics3D::RayOBBDetect(ray, obj->OBB))//(CheckIntersection(mouseRay, mouseRay.GetCurrentRay().Origin, obj))
			{
				if(ray.intersectionDist < shortestDistance)
				{
					hitResult = obj->gameobject;
					shortestDistance = ray.intersectionDist;
				}
			}
		}
		if(hitResult != nullptr)
		{
			return hitResult;
		}
	}
	return nullptr;
}

void OctSpatialPartition::AddObjectToCell(OctNode* cell, MeshRenderer* rb)
{
	if(cell->IsLeaf())
	{
		
	}

	for(OctNode* child : cell->children)
	{
		AddObjectToCell(child, rb);
	}
}

void OctSpatialPartition::PrepareCollisionQuery(OctNode* cell, MouseRay& ray)
{

	//if this node is a leaf go up one step
	if(cell->IsLeaf())
	{
		if (Physics3D::RayOBBDetect(ray, *cell->octBounds))
		{
			rayInstersectionList.push_back(cell);
		}
	}

	//for each child node in the current node check collsions with the ray
	for (OctNode* child : cell->children)
	{
		PrepareCollisionQuery(child, ray);
	}
}


