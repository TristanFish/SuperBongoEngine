#pragma once
#include "math/Vector.h"
#include "imgui/imgui.h"
#include "components/ECS.h"
#include "components/3D/MeshRenderer.h"
#include "core/Timer.h"


namespace CustomUI
{

	class PropertiesPanel
	{
	public:
		PropertiesPanel(GameObject* obj);

		~PropertiesPanel();

		void Render();
		GameObject* selectedObj;
	private:


	};


	//! Performance panel Class
	/*!  This class controls creates & controls the performance Panel UI */
	class PerformancePanel
	{
	private:
		//! FPS Update Speed float
		/*! Controls how fast we want to add a new float into the fpsValues Vector */
		 float fpsUpdateSpeed = 0.5f;

		 //! Initial Speed float
		/*! Is used to reset the "fpsUpdateSpeed" variable once it get to 0 */
		 float initSpeed = fpsUpdateSpeed;

		 //! Initial Speed float
		/*! Is used to reset the "fpsUpdateSpeed" variable once it get to 0 */
		std::vector<float> fpsValues;

		//! Latest FPS float
	   /*! Is used to update the FPS text depending on the "fpsUpdateSpeed" variable */
		int lastestFPS;
	public:

		//! Update Function
	   /*! Updates the needed variables in this class */
		void Update(const float deltatime);

		//! Render Function
	   /*! Renders the needed IMGUI info */
		void Render() const;
	};

}

 class PerformanceMonitor
{
public:
	static void DebugFPS();
	static float GetFPS();

	static int FPSLimit;
	static bool LimitFPS;
};
 
