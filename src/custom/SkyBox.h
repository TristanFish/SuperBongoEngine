#ifndef SKYBOX_H
#define SKYBOX_H

#include "components/ECS.h"
#include "components/Components.h"
#include "custom/Camera.h"


class SkyBox :  public GameObject
{
    unsigned int skyboxVAO, skyboxVBO;
	Shader* shader;

	bool LoadSkyBox(const char* posx, const char* negx, const char* posy, const char* negy, const char* posz, const char* negz);

	//void AttachUniforms() const override;
	//void Init(GameObject* g) override;
	//! Update Function (Inherited from Gameobject)
	void Update(const float deltaTime) override;

	//! Render Function (Inherited from Gameobject)

	//! Handle Events Function (Inherited from Gameobject)
	void HandleEvents(const SDL_Event& event) override;

public:

	GLuint skyboxTextureID;
	SkyBox();
	~SkyBox();

	void Render() const override;

};
#endif
