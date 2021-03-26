#ifndef SKYBOX_H
#define SKYBOX_H

#include "components/ECS.h"
#include "components/Components.h"
#include "custom/Camera.h"


class SkyBox :  public GameObject
{
    unsigned int skyboxVAO, skyboxVBO;
	GLuint skyboxTextureID;
	ShaderProgram shader;
	mutable MATH::Matrix4 viewConvert;

	bool LoadSkyBox(const char* posx, const char* negx, const char* posy, const char* negy, const char* posz, const char* negz);

	//void AttachUniforms() const override;
	//void Init(GameObject* g) override;
	//! Update Function (Inherited from Gameobject)

	//! Render Function (Inherited from Gameobject)

	//! Handle Events Function (Inherited from Gameobject)
	void HandleEvents(const SDL_Event& event) override {}
    MATH::Matrix4 Mat3ToMat4(MATH::Matrix3 _m) const;
public:

	void Update(const float deltaTime) override;
	
	GLuint GetSkyBoxTexture() const;
	unsigned int GetVAO() const;

	SkyBox();
	~SkyBox();

	void Render() const;

};
#endif
