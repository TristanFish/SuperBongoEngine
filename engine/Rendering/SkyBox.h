#ifndef SKYBOX_H
#define SKYBOX_H

#include "glew/glew.h"
#include "graphics/ShaderProgram.h"


class SkyBox
{
    unsigned int skyboxVAO, skyboxVBO;
	GLuint skyboxTextureID;
	ShaderProgram shader;
	mutable MATH::Matrix4 viewConvert;

	bool LoadSkyBox(const char* posx, const char* negx, const char* posy, const char* negy, const char* posz, const char* negz);


    static MATH::Matrix4 Mat3ToMat4(const MATH::Matrix3& _m);
public:
	
	GLuint GetSkyBoxTexture() const;
	unsigned int GetVAO() const;

	SkyBox();
	~SkyBox();

	void Render() const;


};
#endif
