#include "TestModel.h"

using namespace MATH;

TestModel::TestModel(std::string name, Vec3 position)
{
	mr = AddComponent<MeshRenderer>();
	mr->LoadModel("Cube.fbx");
	mr->CreateShader("FogVert.glsl", "FogFrag.glsl");
	mr->renderFlags = RenderProperties::RP_OVERRIDE_RENDERER;

	
	this->name = name;
	transform.SetPos(position);
}

void TestModel::AttachUniforms() const
{
	mr->shader.TakeUniform("Fog.maxDist", 50.0f);
	mr->shader.TakeUniform("Fog.minDist", 10.0f);
	mr->shader.TakeUniform("Fog.color", Vec3(1.0f, 1.0f, 0.0f));
}