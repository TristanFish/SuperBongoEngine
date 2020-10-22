#pragma once
#include "graphics/Model.h"
#include "graphics/Shader.h"
#include "components/ECS.h"



enum class RenderProperties : char
{
	NONE				= 0b00000000,
	LIGHTING			= 0b00000001,
	CREATES_SHADOWS		= 0b00000010,
	RECIEVES_SHADOWS	= 0b00000100,
	BLOOM				= 0b00001000,
	PHYSICS_MOVEMENT	= 0b00010000,
	OVERRIDE_RENDERER	= 0b00100000,
	WATER               = 0b01000000
};

inline constexpr char operator&(RenderProperties rp1, RenderProperties rp2)
{
	return (static_cast<char>(rp1) & static_cast<char>(rp2));
}


class MeshRenderer : public Component
{
public:

	MeshRenderer();
	MeshRenderer(const char* modelPath);
	virtual ~MeshRenderer();

	void CreateShader(const char* vert, const char* frag);

	// Inherited via Component
	void Init(GameObject* g) override;
	void Update(const float deltaTime) override;
	void Render() const override;
	void Render(const Shader& shader) const;
	void HandleEvents(const SDL_Event& event) override;
	
	//IMPORTANT//
	//This function is used to attach any uniforms that are specific to the object being rendered
	//create a definition for this function where you set shader uniforms if
	//you're using the OVERRIDE_RENDERER renderflag for this object
	virtual void AttachUniforms() const {};

	// Getters for Min/Max Vector's
	 Vec3 GetMinVector() const  { return model->p_min; }
	 Vec3 GetMaxVector() const { return model->p_max; }

	 // Getter for meshes
	const std::vector<Mesh>& GetMeshes() const { return model->meshes; };

	void SetInstanceID(const int id)  { instanceID = id; }
	void SetInstanceAmount(const unsigned int amount) { instanceAmount = amount; }

	RenderProperties renderFlags;
	Shader shader;
	Model* model;

private:
	int instanceID;
	unsigned int instanceAmount;
	// Used For Axis Aligned Binding Boxes
};