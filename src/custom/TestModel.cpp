#include "TestModel.h"

using namespace MATH;

TestModel::TestModel(const char* name, MATH::Vec3 position)
{
	mr = AddComponent<MeshRenderer>();
	mr->LoadModel("Cube.fbx");
	mr->CreateShader("src/graphics/shaders/FogVert.glsl", "src/graphics/shaders/FogFrag.glsl");
	mr->renderFlags = RenderProperties::OVERRIDE_RENDERER;
	mr->uaCallback = *this; 

	AddComponent<RigidBody3D>();
	//RigidBody3D::setColliderShape(Collider3D::shape::Sphere);
	//RigidBody3D::ApplyConstantForce(Vec3(0.0, -1.0, 0.0));
	
	this->name = name;
	transform.SetPos(position);
}

void TestModel::operator()()
{
	mr->shader.TakeUniform("Fog.maxDist", 50.0f);
	mr->shader.TakeUniform("Fog.minDist", 10.0f);
	mr->shader.TakeUniform("Fog.color", Vec3(1.0f, 1.0f, 0.0f));
}

const char* TestModel::GetClassIDName() const
{
	return "TestModel";
}





