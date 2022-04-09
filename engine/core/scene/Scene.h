#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "components/SceneGraph.h"
#include "tinyxml2/tinyxml2.h"
#include "core/MouseRay.h"
#include "Utility/SaveUtility.h"



//! Scene Class
/*! Standard scene class that all scenes will inherit from  */
class Scene : public MouseEventListener
{
protected:
	std::string Scene_Name;



public:
	std::shared_ptr<SceneGraph> objectList;
	Scene();
	virtual ~Scene();

	virtual bool OnCreate();
	virtual bool PostCreate();
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime);
	virtual void Render();
	virtual void HandleEvents(const SDL_Event& event);
	void Reset();

	void OnMouseMove(MATH::Vec2 mouse) override;
	void OnMousePressed(MATH::Vec2 mouse, int buttonType) override;
	void OnMouseReleased(MATH::Vec2 mouse, int buttonType) override;

	std::string& GetSceneName()  { return Scene_Name; }
	void SetSceneName(const std::string& Name_) { Scene_Name = Name_; }

};

#endif