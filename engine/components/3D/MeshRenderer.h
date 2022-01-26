#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <functional>

#include "graphics/Model.h"
#include "graphics/ShaderProgram.h"
#include "components/ECS.h"


enum RenderProperties : unsigned short
{
	RP_NONE					= 0b00000000,
	RP_LIGHTING				= 0b00000001,
	RP_CREATES_SHADOWS		= 0b00000010,
	RP_RECIEVES_SHADOWS		= 0b00000100,
	RP_BLOOM				= 0b00001000,
	RP_PHYSICS_MOVEMENT		= 0b00010000,
	RP_TRANSPARENT			= 0b00100000,
	RP_WATER				= 0b01000000,
	RP_OVERRIDE_RENDERER	= 0b10000000
};




struct RenderFlagPair
{
	const char* flagName;
	RenderProperties flagEnum;

	RenderFlagPair(const char* name, RenderProperties flag)
	{
		flagName = name;
		flagEnum = flag;
	}
};

static RenderFlagPair RenderFlagNameEnumPairs[]{RenderFlagPair("None", RP_NONE),
									RenderFlagPair("Lighting", RP_LIGHTING),
									RenderFlagPair("Creates Shadows", RP_CREATES_SHADOWS),
									RenderFlagPair("Recieves Shadows", RP_RECIEVES_SHADOWS),
									RenderFlagPair("Bloom", RP_BLOOM),
									RenderFlagPair("Physics", RP_PHYSICS_MOVEMENT),
									RenderFlagPair("Transparent", RP_TRANSPARENT),
									RenderFlagPair("Water", RP_WATER),
									RenderFlagPair("Overrides Default Renderer", RP_OVERRIDE_RENDERER)};



//!MeshRenderer Class
/*!Allows any gameObject with this component to render a mesh*/
class MeshRenderer : public Component
{
public:
	//!RenderFlags 
	/*!Controls what type of rendering we want to do for this MeshRenderer */
	RenderProperties renderFlags;

	//!Shader
	/*!This is the corresponding shader used in this MeshRenderer*/
	ShaderProgram shader;
	
	MeshRenderer();
	virtual ~MeshRenderer() = default;

	bool LoadModel(const char* name);
	//!Create Shader Function
	/*!Creates a shader when given vertex & fragment file paths*/
	void CreateShader(const char* vert, const char* frag);

	//!Initialize override Function
	/*!Initializes the MeshRenderer*/
	void Init(GameObject* g) override;

	//!Update override Function
	/*!Updates the MeshRenderers systems*/
	void Update(const float deltaTime) override;

	//!Render override Function
	/*!Render the mesh & run the shader*/
	void Render() const override;

	//!Render override Function
	/*!Render the mesh & run the shader that the function is given*/
	void Render(const ShaderProgram& shader_) const;

	//!HandleEvents override Function
	/*!Handles any events needed for the MeshRenderer*/
	void HandleEvents(const SDL_Event& event) override {}

	void OnSaveComponent(const std::string& saveName,std::string parentName) override;

	void ImGuiRender() override;

private:

	//!AttachUniforms function
	/*!Enables the components to know what gameObject they are attached to
	This function is used to attach any uniforms that are specific to the object being rendered
	create a definition for this function where you set shader uniforms if
	you're using the OVERRIDE_RENDERER renderflag for this object*/
	void AttachUniforms() const
	{
		if(uaCallback != nullptr)
		{
			uaCallback();
		}
	}

	void RenderMesh(const Mesh& mesh, const ShaderProgram& shader) const;
	void RenderInstancedMesh(const std::vector<Mesh>& meshes, const ShaderProgram& shader, unsigned int amount) const;

public:
	
	typedef std::function<void ()> UniformAttachCallback;
	UniformAttachCallback uaCallback;

	void AddUniformFunction(const std::function<void ()> &func)
	{
		uaCallback = func;
	}

	 //!MeshColorTint Vec4
	 /*!Stores the color of the mesh*/
	MATH::Vec4 meshColorTint;

	 //!SetColorTint Setter
	 /*!Set's the color of a mesh*/
	 void SetColorTint(const MATH::Vec4 tint_) { meshColorTint = tint_; }

	 //!GetMeshes Getter
	 /*!Returns the vector of meshes*/
	const std::vector<Mesh>& GetMeshes() const { return model->meshes; }

	//!SetInstanceID Setter
	/*!Sets the instanceID variable*/
	Model* GetModel() { return model; }


	//!SetInstanceID Setter
	/*!Sets the instanceID variable*/
	void SetInstanceID(const int id)  { instanceID = id; }

	//!SetInstanceAmount Setter
	/*!Sets the instanceAmount variable*/
	void SetInstanceAmount(const unsigned int amount) { instanceAmount = amount; }


	

private:

	//!Model Pointer
	/*!Holds the model that this MeshRenderer corresponds too*/
	Model* model;

	//!instanceID Integer
	/*!*/
	 int instanceID;

	//!instanceAmount Integer
	/*!Stores how many instances of that object we want to render*/
	unsigned int instanceAmount;

};

#endif