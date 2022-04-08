#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "components/ECS.h"
#include "graphics/ShaderProgram.h"
#include <glew/glew.h>

class Texture;

class SpriteComponent : public Component
{
private:
	Texture* texture;
	GLuint vao, vbo, ebo;
	
public:

	ShaderProgram shader;
	SpriteComponent() = default;
	SpriteComponent(const char* path);

	virtual ~SpriteComponent();

	void setTexture(const char* path);

	void setShaders(const char* vertexPath, const char* fragmentPath);

	 void Init(std::shared_ptr<GameObject> g) override;
	 void Update(const float deltaTime) override;
	 void Render() const;
	 void HandleEvents(const SDL_Event& event) override {}
};
#endif // !1