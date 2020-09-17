#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <sdl/SDL.h>
#include "components/ECS.h"
#include "graphics/shader.h"
#include <glew/glew.h>

class Texture;

class SpriteComponent : public Component
{
private:
	Texture* texture;
	GLuint vao, vbo, ebo;
	
public:

	Shader shader;
	SpriteComponent() = default;
	SpriteComponent(const char* path);

	virtual ~SpriteComponent();

	void setTexture(const char* path);

	void setShaders(const char* vertexPath, const char* fragmentPath);

	 void Init(GameObject *g) override;
	 void Update(const float deltaTime) override;
	 void Render() const override;
	 void HandleEvents(const SDL_Event& event) override;
};
#endif // !1