#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "components/Component.h"
#include "core/Debug.h"

enum class LightType
{
	POINT = 1,
	SPOT,
	DIRECTIONAL
};

struct LightData
{
	MATH::Vec3 ambColor;
	MATH::Vec3 diffColor;
	MATH::Vec3 specColor;
	LightType type;
	float intensity;
	
	float cutOff;
	float outerCutOff;

	float attenConstant;
	float attenLinear;
	float attenQuadratic;

	LightData() = default;

	void SendLightDataToShader(const ShaderProgram& shader, const MATH::Vec3& pos, const MATH::Vec3& direct, const std::string& shaderString) const;

};

struct LightTypePair
{
	const char* typeName;
	LightType typeEnum;

	LightTypePair(const char* name, LightType type)
	{
		typeName = name;
		typeEnum = type;
	}
};

static LightTypePair lightTypeNameEnumPairs[]{LightTypePair("Point", LightType::POINT),
											  LightTypePair("Spot", LightType::SPOT),
											  LightTypePair("Directional", LightType::DIRECTIONAL)};


class LightComponent : public Component
{
public:

	LightData lightInfo;

	virtual ~LightComponent();
	
	// Inherited via Component
	void Init(GameObject* g) override;
	void Update(const float deltaTime) override {}
	void Render() const {}
	void HandleEvents(const SDL_Event& event) override {}

	void OnSaveComponent(const std::string& saveName, std::string parentName) override;

	void ImGuiRender() override;
};
#endif
