#include "Sphere.h"
#include "components/Components.h"
#include "components/3D/lineRenderer.h"

using namespace MATH;

Sphere::Sphere()
{
}

Sphere::Sphere(const std::string& name, MATH::Vec3 position)
{
	AddComponent<MeshRenderer>()->LoadModel("Sphere.fbx");
	MeshRenderer* mr = GetComponent<MeshRenderer>();
	mr->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");
	
	std::vector<Vec3> vertices;
	for(auto& vertex : mr->GetModel()->GetVertices())
	{
		vertices.emplace_back(vertex.position);
	}
	
	AddComponent<RigidBody3D>();
	
	AddComponent<LineRenderer>()->AddPoints(vertices);
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");


	this->name = name;
	transform.SetPos(position);
	canBeInstantiated = true;

}

Sphere::~Sphere()
{
}

void Sphere::OnCollisionEnter(Collider3D& otherBody)
{
	//std::cout << name << " Collided With: " << otherBody.gameObject->name << std::endl;
}

void Sphere::PostInit()
{
	GetComponent<RigidBody3D>()->ConstructCollider(ColliderType::Sphere);
}
