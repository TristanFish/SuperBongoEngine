#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "Ray.h"

#include "core/3D/OctSpatialPartition.h"


class CollisionHandler {
public:
	CollisionHandler(const CollisionHandler&) = delete;
	CollisionHandler(CollisionHandler&&) = delete;
	CollisionHandler& operator=(const CollisionHandler&) = delete;
	CollisionHandler& operator=(CollisionHandler&&) = delete;

	static CollisionHandler* GetInstance();

	void OnCreate(float worldSize_);
	void AddObject(MeshRenderer* go_);
	
	//Was in Shaked's code
	//void MouseUpdate(MATH::Vec2 mousePosition_, int buttonType_);
	
	void OnDestroy();

private:
	CollisionHandler();
	~CollisionHandler();

	static std::unique_ptr<CollisionHandler> collisionInstance;
	friend std::default_delete<CollisionHandler>;

	static std::vector<MeshRenderer*> prevCollisions;
	static OctSpatialPartition* scenePartition;
};

#endif
