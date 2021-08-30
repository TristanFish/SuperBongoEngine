#pragma once
#include <vector>
#include "core/Debug.h"
#include "graphics/ShaderProgram.h"




class SkyBox;
class MeshRenderer;
class LightComponent;


class Renderer
{
public:
	std::vector<MeshRenderer*> meshRenderers;
	std::vector<LightComponent*> lights;
	GLuint gBuffer;

	void Init();
	void AddMeshRenderer(MeshRenderer* mr);
	void AddLight(LightComponent* light);
	void Render() const;
	void DestroyRenderer();
	static SkyBox* GetSkyBox();
	
private:
	ShaderProgram gBufferShader;
	ShaderProgram resultShader;
	GLuint depthRenderBuffer;

	GLuint depthTexture;
	GLuint stencilTexture;
	GLuint posTexture;
	GLuint normTexture;
	GLuint albedoTexture;

	GLuint vao;
	GLuint vbo;

	Debug pos;
	Debug norm;
	Debug albedo;
	Debug depth;
	Debug stencil;

	static inline SkyBox* skyBox;
	//GLuint currentGTexture;

	static bool IsMeshOnScreen(const MeshRenderer& mr);

	void BindGBufferTextures() const;
	void UnbindGBufferTextures() const;

	void RenderGBufferResult() const;


	//Attaches the most important lights to this object's shader
	void AttachLights() const;
	//Draws this object into the shadow texture
	void RenderShadowTexture(const MeshRenderer& mr) const;
	//Draws shadows onto this object
	void RenderShade(const MeshRenderer& mr) const;
	//Does bloom
	void RenderBloom(const MeshRenderer& mr) const;
	//Does physics movement
	void RenderPhysics(const MeshRenderer& mr) const;
	//Does water stuff
	void RenderWaterEffects(const MeshRenderer& mr) const;
		/*NONE			
		LIGHTING		
		CREATES_SHADOWS	
		RECIEVES_SHADOWS
		BLOOM			
		PHYSICS_MOVEMENT */
};

