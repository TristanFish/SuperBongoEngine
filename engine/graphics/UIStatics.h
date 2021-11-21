#ifndef UISTATICS_H
#define UISTATICS_H


#include <memory>
#include <iostream>


#include "math/Vector.h"

class GameObject;
class SceneGraph;
class MeshRenderer;
class UIStatics
{
public:


	UIStatics() = default;
	~UIStatics();





	//! DrawVec3 Function
	/*! Draws a Vec3 to with screen with ImGui */
	static bool DrawVec3(const std::string& label, MATH::Vec3& value, const float columnWidth);


	//!DrawTextureSlot Function
	/*! Will Draw a textured image that can be used to change a mesh/texture on drag drop if meshRenderer is overridden */
	static void DrawTextureSlot( const char* textureName, MeshRenderer* meshRenderer = nullptr ,const float spacing = 15.0f, const MATH::Vec2& size = MATH::Vec2(64.0f, 64.0f));

	static void SetSelectedObject(GameObject* go) { selectedObj = go; }



	static GameObject* GetSelectedObject() { return selectedObj; }


private:

	//! static GameObject Pointer
	/*! Hold's the object that was selected last */
	static GameObject* selectedObj;





};

#endif