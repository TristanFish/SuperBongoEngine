#ifndef CUSTOMUI_H
#define CUSTOMUI_H

#include "components/GameObject.h"
#include "components/3D/MeshRenderer.h"



//! CustomUI namespace
/*!  This namespace holds all of our custom IMGUI functionalities */
namespace CustomUI
{
	//! Properties panel Class
	/*!  This class creates & controls the Properties Panel UI for each gameobject */
	class PropertiesPanel
	{
	public:

		//! Properties panel Constructor
		/*!  Initializes the variables that are needed for this class */
		PropertiesPanel(GameObject* obj);

		//! Properties panel Destructor
		/*! Destroys all pointer's that this class uses */
		~PropertiesPanel();

		//! Render Function
		/*! Renders the IMGUI UI on screen */
		void Render() const;

		//! GameObject Pointer
		/*! Is a pointer to the selected object this panel uses */
		GameObject* selectedObj;

	};


	//! Performance panel Class
	/*!  This class creates & controls the performance Panel UI */
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
		int latestFPS = 0;
	public:
		PerformancePanel();
		~PerformancePanel();


		//! Update Function
	   /*! Updates the needed variables in this class */
		void Update(const float deltatime);

		//! Render Function
	   /*! Renders the needed IMGUI info */
		void Render() const;
	};

	class HierarchyPanel
	{
	public:

		HierarchyPanel() = default;

		~HierarchyPanel();

		void ConstructHierarchy(std::vector<GameObject*>& allParentObjects);
		void Update();
		void Render() const;
	
	private:
		std::vector<GameObject*> gameobjects;

		void DisplayGameObjectInHierarchy(GameObject* go) const;

	};

}


//! Performance Monitor Class
/*!  This class calculates all the performance variables */
class PerformanceMonitor
{
public:
	
	//! Initialize Monitor Function
	/*!  This function initializes the non-static member's of the Performance Monitor Class  */
	static void InitMonitor();

	//! Debug Frames Per Second Function
	/*!  Debugs the FPS to the console  */
	static void DebugFPS();

	//! Get Frames Per Second Function
	/*!  Returns Current FPS  */
	static float GetFPS();

	//! Get Memory Usage Function
	/*!  Returns the current amount of physical ram used by the application  */
	static int GetMemoryUsage();

	//! Get CPU Usage Function
	/*!  Returns percent of CPU usage the application is using  */
	static double GetCPUUsage();


	//! Frames Per Second Integer
	/*!  This variable stores the user inputed FPS limit  */
	static int FPSLimit;

	//! Limit Frames Per Second boolean
	/*!  This variable toggles if the FPSLimit variable is used  */
	static bool LimitFPS;

	//! Render Loop Time Float
	/*!  This variable contains how faster our application is rendering in milliseconds */
	static float RenderLoopTime;

	//! Update Loop Time Float
	/*!  This variable contains how faster our application is updating in milliseconds */
	static float UpdateLoopTime;
};
 
#endif