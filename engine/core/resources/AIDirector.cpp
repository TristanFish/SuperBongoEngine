#include "AIDirector.h"


#include "math/VMath.h"

#include "core/Globals.h"
#include "components/SceneGraph.h"
#include "components/3D/MeshRenderer.h"


std::unique_ptr<AIDirector> AIDirector::directorInstance = std::unique_ptr<AIDirector>();


AIDirector::AIDirector()
{
	UM_PathAlgorithms.emplace(PathfindingAlgorithm::ASTAR, new AStar());
	UM_PathAlgorithms.emplace(PathfindingAlgorithm::BREADTHFIRST, new BreadthFirst());
}

AIDirector::~AIDirector()
{

	for (auto Algorithm : UM_PathAlgorithms)
	{
		delete Algorithm.second;
		Algorithm.second = nullptr;
	}

}

AIDirector* AIDirector::GetInstance()
{
	if (directorInstance == nullptr)
	{
		directorInstance.reset(new AIDirector);
	}
	return directorInstance.get();
}

std::vector<MATH::Vec3> AIDirector::GetPositiveVerticies() const
{
	std::vector<MATH::Vec3> positiveVerticies;

	for (const auto& obj : Globals::s_SceneGraph->GetGameObjects())
	{
		for (const auto& vertex : obj->GetComponent<MeshRenderer>()->GetModel()->GetVerticies())
		{
			if (MATH::VMath::dot(vertex.normal, MATH::Vec3::Up()) > 0.0f)
			{
				positiveVerticies.push_back(obj->transform.GetModelMatrix() * vertex.position);
			}
		}
	}

	return positiveVerticies;
}



void AIDirector::FindPath(Graph* traverableGraph, PathfindingAlgorithm Algorithm)
{
	UM_PathAlgorithms[Algorithm]->SetCurrentGraph(traverableGraph);
	UM_PathAlgorithms[Algorithm]->CalculatePath();
}
