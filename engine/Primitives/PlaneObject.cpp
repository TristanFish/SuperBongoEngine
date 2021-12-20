#include "PlaneObject.h"

using namespace MATH;



PlaneObject::PlaneObject(const std::string& name, Vec3 position)
{
	AddComponent<MeshRenderer>()->LoadModel("Plane.fbx");
	GetComponent<MeshRenderer>()->CreateShader("DefaultVert.glsl", "DefaultFrag.glsl");
	
	
	this->name = name;
	transform.SetPos(position);

	transform.SetRot(Vec3(1.0f, 1.0f, 1.0f));
	

	canBeInstantiated = true;

}

void PlaneObject::OnCollisionEnter(Collider3D& otherBody)
{
	std::cout << "PlaneObject Collided with something" << std::endl;
}