#ifndef UISTATICS_H
#define UISTATICS_H


#include <memory>

class GameObject;
class SceneGraph;

class UIStatics
{
public:

	UIStatics() = default;
	~UIStatics();


	static inline std::shared_ptr<SceneGraph> GetSceneGraph() { return sceneGraph; }

	static inline GameObject* GetSelectedObject() { return selectedObj; }

	static inline void SetSelectedObject(GameObject* go) { selectedObj = go; }

	static void ConstructUIStatics();

private:

	//! static GameObject Pointer
	/*! Hold's the object that was selected last */
	static GameObject* selectedObj;


	static std::shared_ptr<SceneGraph> sceneGraph;



};

#endif