#ifndef SCENE1_H
#define SCENE1_H
#include "Scene.h"
#include "graphics/shader.h"
#include "graphics/TextureManager.h"
class Scene1 : public Scene
{
private:
	TextureManager* texture;

public:
	Scene1();
	~Scene1();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event &event) override;


};

#endif