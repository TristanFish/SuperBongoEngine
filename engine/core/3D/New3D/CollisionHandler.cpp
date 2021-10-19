#include "CollisionHandler.h"
#include "../Core/CoreEngine.h"
#include "core/Globals.h"

std::unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
std::vector<MeshRenderer*> CollisionHandler::prevCollisions = std::vector<MeshRenderer*>();
OctSpatialPartition* CollisionHandler::scenePartition = nullptr;


CollisionHandler::CollisionHandler() {
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

void CollisionHandler::AddObject(MeshRenderer* go_) {
	if (scenePartition != nullptr) { 
		scenePartition->AddObject(go_);
	}
}
