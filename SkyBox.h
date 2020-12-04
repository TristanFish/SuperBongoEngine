#ifndef SKYBOX_H
#define SKYBOX_H

#include "components/ECS.h"
#include "custom/Camera.h"
#include "graphics/Mesh.h"
#include <sdl/SDL.h>

class SkyBox: public GameObject
{
	GLuint skyboxTextureID;
	GLuint cubeVAO;
	Mesh* meshPtr;
	Shader* shaderPtr;
	Texture* texturePtr;


	bool LoadSkyBox(const char* posx, const char* negx, const char* posy, const char* negy, const char* posz, const char* negz);

	//! Update Function (Inherited from Gameobject)
	void Update(const float deltaTime) override;

	//! Render Function (Inherited from Gameobject)
	void Render() const override;

	//! Handle Events Function (Inherited from Gameobject)
	void HandleEvents(const SDL_Event& event) override;

};
#endif
