#pragma once

#include "components/Components.h"
#include "glew/glew.h"
#include "custom/SkyBox.h"

class SkyBox;

class Water : public GameObject, public MeshRenderer
{
public:
	
	
	const void GetSkyBoxInfo();
	void CreateQuad();
	void Render() const override;
	Water(const char* name, MATH::Vec3 pos, SkyBox* _skybox);
	~Water();
private:

	unsigned int reflectionFrameBuffer;
	unsigned int reflectionTexture;
	unsigned int reflectionDepthBuffer;

	GLuint skyboxID;;
	unsigned int skyBoxVAO;
	SkyBox* skyBox = nullptr;

	void Update(const float deltaTime) override;
	void HandleEvents(const SDL_Event& event) override;


};

