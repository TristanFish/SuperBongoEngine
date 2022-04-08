#pragma once

#include <vector>
#include "Vector.h"
#include "components/Component.h"

#include "glew/glew.h"
class LineRenderer : public Component
{
public:
	
	std::vector<MATH::Vec3> points;
	MATH::Vec4 color;
	float lineWidth;
	bool looped;

	LineRenderer();
	void Init(std::shared_ptr<GameObject> g) override;
	void Update(const float deltaTime) override;
	void ImGuiRender() override;
	virtual ~LineRenderer() override;
	
	void AddPoints(const std::vector<MATH::Vec3>& pointsToAdd);
	void AddPoint(const MATH::Vec3& pointToAdd);

	void RenderLine() const;

private:

	GLuint vao;
	GLuint vbo;

	void SetUpBuffers();
	void DeleteBuffers();
public:
	void HandleEvents(const SDL_Event& event) override {};
};