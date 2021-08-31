#include "PlaneObject.h"

using namespace MATH;



PlaneObject::PlaneObject(std::string name, Vec3 position)
{
	AddComponent<MeshRenderer>()->LoadModel("Plane.fbx");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");
	
	//AddComponent<RigidBody3D>()->collisionEnterCallback = std::bind(&PlaneObject::OnCollisionEnter, this, std::placeholders::_1);
	
	this->name = name;
	transform.SetPos(position);

	transform.scale =  Vec3(1.0f, 1.0f, 1.0f);
	//transform.rotation = Vec3(90.0f, 0.0f, 0.0f);
	transform.rotation.print();

	canBeInstantiated = true;

}

void PlaneObject::OnCollisionEnter(RigidBody3D& otherBody)
{
	std::cout << "PlaneObject Collided with something" << std::endl;
}