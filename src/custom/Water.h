#pragma once

#include "components/Components.h"
#include "glew/glew.h"
#include "custom/SkyBox.h"
#include "core/Globals.h"
#include "core/Debug.h"

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

	static const int REFLECTION_WIDTH = 1280;
	static const int REFLECTION_HEIGHT = 720;

	static const int REFRACTION_WIDTH = 1280;
	static const int REFRACTION_HEIGHT = 720;

	unsigned int reflectionFrameBuffer;
	unsigned int reflectionDepthBuffer;
	unsigned int reflectionTexture;

	unsigned int refractionFrameBuffer;
	unsigned int refractionDepthTexture;
	unsigned int refractionTexture;

	GLuint skyboxID;
	unsigned int skyBoxVAO;
	SkyBox* skyBox = nullptr;
	float time;

	void BindReflectionFrameBuffer() const;
	void BindRefractionFrameBuffer() const;
	void CreateReflectionFrameBuffer();
	void CreateRefractionFrameBuffer();
	void CreateTextureAttachment(unsigned int texture, int width, int height);
	void CreateDepthBufferAttachment();
	void CreateDepthTextureAttachment();
	void initReflectionFB();
	void initRefractionFB();
	void UnbindCurrentFB() const;


	void Update(const float deltaTime) override;
	void HandleEvents(const SDL_Event& event) override;


};

