#ifndef RENDERER_H
#define RENDERER_H

#include <vector>


#include "core/Debug.h"
#include "graphics/ShaderProgram.h"
#include "graphics/CustomUI.h"
#include "graphics/FrameBuffer.h"





class SkyBox;
class MeshRenderer;
class LightComponent;
class Renderer
{
public:
	std::vector<MeshRenderer*> meshRenderers;
	std::vector<LightComponent*> lights;
	
	//framebuffers
	FrameBuffer defaultBuffer;
	FrameBuffer gBuffer;
	FrameBuffer gBufferRenderResult;

	void Init();
	void SetupFrameBuffers();
	void SetupTextures();

	void BindDefaultBuffer() { defaultBuffer.Bind(); }
	void ClearDefaultBuffer() { defaultBuffer.Clear(); }
	
	void AddMeshRenderer(MeshRenderer* mr);
	void DeleteMeshRenderer(MeshRenderer* mr);
	void AddLight(LightComponent* light);
	void DeleteLight(LightComponent* light);

	static void DrawDebugGeometry(const std::vector<GameObject*>& objects);

	void Render();
	void DestroyRenderer();

	static Renderer* GetInstance();
	void ClearComponents();

	static SkyBox* GetSkyBox();
	
	GLuint GetModeTextureID() const;

	GLuint GetgBufferTextureID() const { return albedoTexture.texture; }

	void Resize(const int size_x, const int size_y);

private:


	

	// Viewport 
	/*! Handles all of the needed functions for the viewport */
	 CustomUI::Viewport viewport;

	ShaderProgram gBufferShader;
	ShaderProgram resultShader;
	GLuint depthRenderBuffer;

	//textures
	BufferTexture gBufferTexture;
	
	BufferTexture depthTexture;
	BufferTexture stencilTexture;
	BufferTexture posTexture;
	BufferTexture normTexture;
	BufferTexture albedoTexture;

	GLuint vao;
	GLuint vbo;

	//debugging Textures
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
