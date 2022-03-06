#include "AIDirector.h"


#include "math/VMath.h"

#include "core/Globals.h"

#include "components/SceneGraph.h"
#include "components/3D/MeshRenderer.h"
#include "components/GameObject.h"



std::unique_ptr<AIDirector> AIDirector::directorInstance = std::unique_ptr<AIDirector>();


AIDirector::AIDirector() : G_RecentlyGenerated(nullptr)
{
	UM_PathAlgorithms.emplace(PathfindingAlgorithm::ASTAR, new AStar());
	UM_PathAlgorithms.emplace(PathfindingAlgorithm::BREADTHFIRST, new BreadthFirst());

	UM_DivisionAlgorithms.emplace(DivisionAlgorithm::NAIVE, new NaiveVisAlgo());
	NavMesh = std::make_shared<NavigationMesh>();

}

AIDirector::~AIDirector()
{

	for (auto PathAlgorithm : UM_PathAlgorithms)
	{
		delete PathAlgorithm.second;
		PathAlgorithm.second = nullptr;
	}

	for (auto DivAlgorithm : UM_DivisionAlgorithms)
	{
		delete DivAlgorithm.second;
		DivAlgorithm.second = nullptr;
	}

	G_RecentlyGenerated = nullptr;
}

AIDirector* AIDirector::GetInstance()
{
	if (directorInstance == nullptr)
	{
		directorInstance.reset(new AIDirector);
	}
	return directorInstance.get();
}

std::vector<Poly> AIDirector::GetPositiveVerticies() const
{
	std::vector<Poly> positivePolygons;

	for (const auto& obj : Globals::Engine::s_SceneGraph->GetGameObjects())
	{
		if (!obj->HasComponent<MeshRenderer>())
			continue;

		Poly New_Polygon;
		for (const auto& vertex : obj->GetComponent<MeshRenderer>()->GetModel()->GetVerticies())
		{

			MATH::Vec3 RotNorm = obj->transform.GetRotationQuat().Rotate(vertex.normal);

			float Result = MATH::VMath::dot(RotNorm, MATH::Vec3::Up());
			float ResultAngle = acos(Result) / (MATH::VMath::mag(RotNorm) * MATH::VMath::mag(MATH::Vec3::Up()));
			ResultAngle *= 180.0f / 3.14159f;

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


void AIDirector::GenerateGraphFromMap(DivisionAlgorithm Algorithm)
{
	UM_DivisionAlgorithms[Algorithm]->UpdatePolygons(GetPositiveVerticies());
	G_RecentlyGenerated = UM_DivisionAlgorithms[Algorithm]->ConstructDivision();
}




std::vector<MATH::Vec3> AIDirector::FindPath(Node* N_Target, Graph* traverableGraph, PathfindingAlgorithm Algorithm)
{
	std::vector<MATH::Vec3> World_Path;

	if (traverableGraph != nullptr)
	{
		UM_PathAlgorithms[Algorithm]->SetCurrentGraph(traverableGraph);
	}
	else
	{
		UM_PathAlgorithms[Algorithm]->SetCurrentGraph(G_RecentlyGenerated);
	}
	UM_PathAlgorithms[Algorithm]->SetGoal(N_Target);
	UM_PathAlgorithms[Algorithm]->CalculatePath();


	for (auto node : UM_PathAlgorithms[Algorithm]->GetCalculatedPath())
	{
		World_Path.push_back(node->V_WorldPos);
	}

	return World_Path;
}


Graph* AIDirector::GetRecentlyGenerated() const
{
	return G_RecentlyGenerated;
}
