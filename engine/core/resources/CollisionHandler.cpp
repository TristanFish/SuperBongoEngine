#include "CollisionHandler.h"
#include "../Core/CoreEngine.h"
#include "core/Globals.h"

#include "core/3d/Physics/Collider3D.h"

std::unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;


CollisionHandler::CollisionHandler() : prevCollisions(std::vector<Collider3D*>()), scenePartition(nullptr) {
	prevCollisions.reserve(10);
}

CollisionHandler::~CollisionHandler() {
	OnDestroy();
}

CollisionHandler* CollisionHandler::GetInstance() {
	if (collisionInstance.get() == nullptr) { 
		collisionInstance.reset(new CollisionHandler);
	}
	return collisionInstance.get();
}

void CollisionHandler::OnCreate(float worldSize_) {
	prevCollisions.clear();
	scenePartition = new OctSpatialPartition(worldSize_);
}

void CollisionHandler::OnDestroy() {
	delete scenePartition;
	scenePartition = nullptr;

	for (auto* entry : prevCollisions) {
		entry = nullptr;
	}
	prevCollisions.clear();
}

void CollisionHandler::AddObject(Collider3D* collider) {
	if (scenePartition != nullptr) { 
		scenePartition->AddObject(collider);
	}
}
