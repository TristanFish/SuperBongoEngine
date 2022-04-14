#include "lineRenderer.h"

#include "Rendering/CameraManager.h"

#include "components/GameObject.h"

#include "core/resources/ShaderManager.h"

#include "graphics/ShaderProgram.h"

LineRenderer::LineRenderer()
{
	vao = 0;
	vbo = 0;
	lineWidth = 5.0f;
	looped = true;
	color = MATH::Vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

void LineRenderer::Init(std::shared_ptr<GameObject> g)
{
	gameObject = g;

	Renderer::GetInstance()->AddLine(this);
}

void LineRenderer::Update(const float deltaTime)
{
}

void LineRenderer::ImGuiRender()
{
	const bool opened = Globals::Editor::OpenComponentTreeNode(this, "Line Renderer");

	if(opened)
	{
		ImGui::Checkbox("Looped", &looped);
		ImGui::DragFloat("Line Width", &lineWidth, 0.2f, 0.0f, 10.0f);
		ImGui::TreePop();
	}
}

LineRenderer::~LineRenderer()
{
	Renderer::GetInstance()->DeleteLine(this);
}

void LineRenderer::AddPoints(const std::vector<MATH::Vec3>& pointsToAdd)
{
	points.insert(points.end(), pointsToAdd.begin(), pointsToAdd.end());
	SetUpBuffers();
}

void LineRenderer::AddPoint(const MATH::Vec3& pointToAdd)
{
	points.emplace_back(pointToAdd);
	SetUpBuffers();
}

void LineRenderer::RenderLine() const
{
	static ShaderProgram shader = ShaderManager::GetShaders("DefaultVert.glsl", "DefaultFrag.glsl");
	
	shader.RunShader();
	shader.TakeUniform("projectionMatrix", CameraManager::GetInstance()->GetCamera()->getProjectionMatrix());
	shader.TakeUniform("viewMatrix", CameraManager::GetInstance()->GetCamera()->getViewMatrix());
	shader.TakeUniform("modelMatrix", gameObject->GetModelMatrix());
	
	shader.TakeUniform("meshColor", color);

	glBindVertexArray(vao);
	glLineWidth(lineWidth);
	if(!looped)
	{
		glDrawArrays(GL_LINES, 0, points.size());
	} else
	{
		glDrawArrays(GL_LINE_LOOP, 0, points.size());
	}
	glLineWidth(1);
	
	glBindVertexArray(0);

	glUseProgram(0);
}

void LineRenderer::SetUpBuffers()
{
	if(vao != 0)
	{
		DeleteBuffers();
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(MATH::Vec3), &points[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MATH::Vec3), reinterpret_cast<void*>(0));
	
	glBindVertexArray(0);
}

void LineRenderer::DeleteBuffers()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	vao = 0;
	vbo = 0;
}
