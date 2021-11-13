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

	UM_DivisionAlgorithms.emplace(DivisionAlgorithm::NAIVE, new NaiveVisAlgo());
	NavMesh = std::make_shared<NavigationMesh>();

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

void AIDirector::GenerateGraphFromMap(DivisionAlgorithm Algorithm)
{

	UM_DivisionAlgorithms[Algorithm]->UpdatePolygons(GetPositiveVerticies());

	UM_DivisionAlgorithms[Algorithm]->ConstructDivision();
}


std::vector<Poly> AIDirector::GetPositiveVerticies() const
{
	std::vector<Poly> positivePolygons;

	for (const auto& obj : Globals::s_SceneGraph->GetGameObjects())
	{
		if (!obj->HasComponent<MeshRenderer>())
			continue;

		Poly New_Polygon;
		for (const auto& vertex : obj->GetComponent<MeshRenderer>()->GetModel()->GetVerticies())
		{

			MATH::Vec3 RotNorm = obj->transform.GetRotationQuat().Rotate(vertex.normal);

			float Result = MATH::VMath::dot(RotNorm, MATH::Vec3::Up());
			float ResultAngle = acos(Result) / (MATH::VMath::mag(RotNorm) * MATH::VMath::mag(MATH::Vec3::Up()));
			ResultAngle *= 180.0f / 3.14159;

			if (ResultAngle < 45.0f)
			{
				New_Polygon.AddVertex((obj->transform.GetModelMatrix() * vertex.position));
			}
		}

		if (!New_Polygon.IsEmpty())
		{
			positivePolygons.push_back(New_Polygon);
		}
	}

	return positivePolygons;
}


void AIDirector::FindPath(Graph* traverableGraph, PathfindingAlgorithm Algorithm)
{
	UM_PathAlgorithms[Algorithm]->SetCurrentGraph(traverableGraph);
	UM_PathAlgorithms[Algorithm]->CalculatePath();
}
