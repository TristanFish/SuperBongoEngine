#ifndef SCENE_H
#define SCENE_H

#include "components/SceneGraph.h"
#include "tinyxml2/tinyxml2.h"
#include "primitives/Primitives.h"
#include "core/MouseRay.h"
#include "sdl/SDL.h"
#include "Utility/SaveUtility.h"
#include "Utility/LoadUtility.h"


#include "graphics/CustomUI.h"
#include <memory>



//! Scene Class
/*! Standard scene class that all scenes will inherit from  */
class Scene : public MouseEventListener
{
protected:
	//! MouseRay
	/*! Converts mouse position on screen to world space to allow us to do mouse picking  */
	MouseRay mouseRay;

	

	CustomUI::DockSpace dockSpace;
	std::string Scene_Name;


	//! Create object with object ID function
	/*! Used when we want to runtime spawn objects depending on the given ID */
	void CreateObjWithID(const MATH::Vec3& pos_, const MATH::Vec3& rot_, const MATH::Vec3& scale_, const std::string& objName_, const std::string& objType) const;


	//! Check Intersection function
	/*! Checks if the MouseRay has intersected with a object */
	bool CheckIntersection(const MouseRay& ray, const MATH::Vec3& origin, GameObject* obj) const;


public:
	std::shared_ptr<SceneGraph> objectList;
	// Used for saving and loading xml document information
	Scene();
	virtual ~Scene();

	virtual bool OnCreate() = 0;
	virtual bool PostCreate();
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime);
	virtual void Render();
	virtual void HandleEvents(const SDL_Event& event);
	virtual void Reset() = 0;
	virtual void SaveMapData() const;
	virtual void LoadMapData();



	void OnMouseMove(MATH::Vec2 mouse) override;
	void OnMousePressed(MATH::Vec2 mouse, int buttonType) override;

	std::string& GetSceneName()  { return Scene_Name; }
	void SetSceneName(const std::string& Name_) { Scene_Name = Name_; }

};

#endif