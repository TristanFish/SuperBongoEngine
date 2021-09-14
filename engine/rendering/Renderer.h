#ifndef RENDERER_H
#define RENDERER_H

#include <vector>


#include "core/Debug.h"
#include "graphics/ShaderProgram.h"
#include "graphics/CustomUI.h"





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
	void DeleteMeshRenderer(MeshRenderer* mr);
	void AddLight(LightComponent* light);
	void DeleteLight(LightComponent* light);


	void Render() ;
	void DestroyRenderer();

	static Renderer* GetInstance();


	static SkyBox* GetSkyBox();
	
	GLuint GetModeTextureID() const;


	void Resize(const int size_x, const int size_y);

private:


	

	// Viewport 
	/*! Handles all of the needed functions for the viewport */
	 CustomUI::Viewport viewport;

	ShaderProgram gBufferShader;
	ShaderProgram resultShader;
	GLuint depthRenderBuffer;

	GLuint depthTexture;
	GLuint gBufferTexture;

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


	static std::unique_ptr<Renderer> rendererInstance;
	friend std::default_delete<Renderer>;

	void BindGBufferTextures() const;
	void UnbindGBufferTextures() const;

	void RenderGBufferResult() ;


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
#endif
