#include "Water.h"


Water::Water(const char* name, MATH::Vec3 pos, SkyBox* _skybox):MeshRenderer("Cube.fbx"){
	this->name = name;
	this->transform.pos = pos;
	MeshRenderer::Init(this);
	MeshRenderer::renderFlags = RenderProperties::OVERRIDE_RENDERER;
	MeshRenderer::CreateShader("src/graphics/shaders/WaterVert.glsl", "src/graphics/shaders/WaterFrag.glsl");
	MeshRenderer::SetInstanceID(0);
	
}

Water::~Water()
{

}


const void Water::GetSkyBoxInfo()
{
	if (skyBox == nullptr) { skyBox = Renderer::GetSkyBox(); }
	skyboxID = skyBox->GetSkyBoxTexture();
	skyBoxVAO = skyBox->GetVAO();
}

void Water::Update(const float deltaTime)
{
}

void Water::Render() const
{
	
	MeshRenderer::shader.RunShader();
	glBindVertexArray(skyBoxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
	MeshRenderer::shader.TakeInUniformMat4("cube", Camera::getInstance()->getViewMatrix());
	MeshRenderer::shader.TakeInUniformVec3("cameraPos", Camera::getInstance()->getPosition());
	MeshRenderer::Render();
	
}

void Water::HandleEvents(const SDL_Event& event)
{
}
