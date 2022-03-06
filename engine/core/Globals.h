#pragma once

#include <iostream>
#include <filesystem>
#include <memory>
#include "components/SceneGraph.h"
#include "math/Vector.h"


class SceneGraph;
class GameObject;
class MeshRenderer;
class Component;

namespace Globals
{
	struct Engine
	{
		static std::shared_ptr<SceneGraph> s_SceneGraph;


		static const int SCREEN_WIDTH = 1536;
		static const int SCREEN_HEIGHT = 864;


		static std::string SCENE_NAME;

		static std::string ENGINE_PATH;
		static std::string SAVE_DATA_PATH;



		static std::string IntToVectorWithPrefix(const char* typePrefix, const int i);

		static std::string IntToVector(const int i);

		static SceneGraph* GetSceneGraph();

		static void InitGlobals();

	};

	struct Editor
	{
	public:

		//! DrawVec3 Function
		/*! Draws a Vec3 to with screen with ImGui */
		static bool DrawVec3(const std::string& label, MATH::Vec3& value, const float columnWidth);

		//!DrawTextureSlot Function
		/*! Will Draw a textured image that can be used to change a mesh/texture on drag drop if meshRenderer is overridden */
		static void DrawTextureSlot(const char* textureName, MeshRenderer* meshRenderer = nullptr, const float spacing = 15.0f, const MATH::Vec2& size = MATH::Vec2(64.0f, 64.0f));

		static void SetSelectedObject(std::shared_ptr<GameObject> go) { selectedObj = go; }

		static bool OpenComponentTreeNode(Component* comp, const char* name);

		static std::shared_ptr<GameObject> GetSelectedObject() { return selectedObj; }


		static int GizmoType;

	private:

		//! static GameObject Pointer
		/*! Hold's the object that was selected last */
		static std::shared_ptr<GameObject> selectedObj;


	};
}

