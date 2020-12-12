#pragma once
#include "graphics/Model.h"
#include "graphics/Shader.h"
#include "components/ECS.h"

enum class RenderProperties : unsigned char
{
	NONE				= 0b00000000,
	LIGHTING			= 0b00000001,
	CREATES_SHADOWS		= 0b00000010,
	RECIEVES_SHADOWS	= 0b00000100,
	BLOOM				= 0b00001000,
	PHYSICS_MOVEMENT	= 0b00010000,
	TRANSPARENT			= 0b00100000,
	WATER				= 0b01000000,
	OVERRIDE_RENDERER	= 0b10000000


};

inline constexpr char operator&(RenderProperties rp1, RenderProperties rp2)
{
	return (static_cast<char>(rp1) & static_cast<char>(rp2));
}


//!MeshRenderer Class
/*!Allows any gameobject with this component to render a mesh*/
class MeshRenderer : public Component
{
public:
	//!RenderFlags 
	/*!Control's what type of rendering we want to do for this MeshRenderer */
	RenderProperties renderFlags;

	//!Shader
	/*!This is the corresponding shader used in this MeshRenderer*/
	Shader shader;

	//!MeshRenderer Constructor
	/*!Empty Function*/
	MeshRenderer();
	//!Alternate MeshRenderer Constructor
	/*!Initializes the render flags in model this MeshRenderer is using*/
	MeshRenderer(const char* modelPath);

	//!Virtual MeshRenderer Destructor
	/*!Destroys any pointers/vectors the MeshRenderer class uses*/
	virtual ~MeshRenderer();

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
	void Render(const Shader& shader) const;

	//!HandleEvents override Function
	/*!Handles any events needed for the MeshRenderer*/
	void HandleEvents(const SDL_Event& event) override;
	
	//!AttachUniforms function
	/*!Enables the components to know what gameobject they are attached too
	This function is used to attach any uniforms that are specific to the object being rendered
	create a definition for this function where you set shader uniforms if
	you're using the OVERRIDE_RENDERER renderflag for this object*/
	virtual void AttachUniforms() const {};

	//!GetMinVector Getter
	/*!Returns the meshes minimum bounding vector*/
	 Vec3 GetMinVector() const  { return model->p_min; }

	 //!GetMaxVector Getter
	 /*!Returns the meshes maximum bounding vector*/
	 Vec3 GetMaxVector() const { return model->p_max; }

	 //!MeshColorTint Vec4
	 /*!Stores the color of the mesh*/
	 Vec4 meshColorTint = Vec4(1.0);

	 //!SetColorTint Setter
	 /*!Set's the color of a mesh*/
	 void SetColorTint(const Vec4 tint_) { meshColorTint = tint_; }

	 //!GetMeshes Getter
	 /*!Returns the vector of meshes*/
	const std::vector<Mesh>& GetMeshes() const { return model->meshes; };

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