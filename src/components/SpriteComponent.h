#pragma once
#include <sdl/SDL.h>
#include "Components.h"
#include "graphics/TextureManager.h"
#include "graphics/shader.h"
#include <glew/glew.h>

class SpriteComponent : public Component
{
private:
	TextureManager* texture;
	Shader shader;

	GLuint vao, vbo, ebo;
	GLuint shader_program;

public:
	SpriteComponent() = default;
	SpriteComponent(const char* path);

	void setTexture(const char* path);

	void setShaders(const char* vertexPath, const char* fragmentPath);

	 void Init() override;
	 void Update(const float deltaTime) override;
	 void Render() const override;
	 void HandleEvents(const SDL_Event& event) override;
};