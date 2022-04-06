#ifndef CUSTOMUI_H
#define CUSTOMUI_H

#include <filesystem>
#include <unordered_map>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>

#include "core/networking/NetworkManager.h"
#include "math/Vector.h"

class GameObject;

//! CustomUI namespace
/*!  This namespace holds all of our custom IMGUI functionalities */
namespace CustomUI
{

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

		//! Render Loop Time Float
		/*!  This variable contains how faster our application is rendering in milliseconds */
		static float RenderLoopTime;

		//! Update Loop Time Float
		/*!  This variable contains how faster our application is updating in milliseconds */
		static float UpdateLoopTime;
	};


	class UIInterface
	{
	public:

		virtual ~UIInterface() = default;
		
		virtual void Render() = 0;
		virtual void Update(const float deltatime) {}
		virtual void Construct() {}
		virtual void Reset() {}
	};

	class NetworkPanel : public UIInterface
	{
	public:
		NetRole role;
		bool roleIsSet = false;
		bool isConnected = false;
		
		
		void SetNetworkRole(NetRole role);
		void Disconnect();
		
		void Render() override;
		
	};
	

	//! Properties panel Class
	/*!  This class creates & Renders the Properties Panel UI for each gameObject */
	class PropertiesPanel : public UIInterface
	{
	public:

		//! Properties panel Constructor
		/*!  Initializes the variables that are needed for this class */
		PropertiesPanel();

		//! Properties panel Destructor
		/*! Destroys all pointer's that this class uses */
		virtual ~PropertiesPanel();

		//! Render Function
		/*! Renders the IMGUI UI on screen */
		void Render() override;

		

	private:

		//! isActive bool
		/*! Hold's if the properties panel is being rendered or not */
		bool isActive;
	};



	//! Performance panel Class
	/*!  This class creates & Renders the performance Panel UI */
	class PerformancePanel : public UIInterface
	{
	private:
		//! FPS Update Speed float
		/*! Controls how fast we want to add a new float into the fpsValues Vector */
		 float fpsUpdateSpeed = 0.1f;

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
		virtual ~PerformancePanel();


		//! Update Function
	   /*! Updates the needed variables in this class */
		void Update(const float deltatime) override;

		//! Render Function
	   /*! Renders the needed IMGUI info */
		void Render() override;
	};


	
	//! Hierarchy panel Class
	/*!  This class creates & Renders the hierarchy Panel UI */
	class HierarchyPanel : public UIInterface
	{
	public:

		HierarchyPanel();

		virtual ~HierarchyPanel();

		//! ConstructHierarchy Function
		/*! Copies over the passed in vector of gameobjects */
		void Construct() override;

		//! Render Function
		/*! Renders all of the supplied IMGui panels */
		void Render() override;
	
		void Reset() override;

		void Update(const float deltatime) override;

	private:

		//! isActive bool
		/*! Hold's if the hierarchy panel is being rendered or not */
		bool isActive;

		//! gameobjects vector
		/*! Holds all of the gameobjects in the hierarchy */
		std::vector<std::shared_ptr<GameObject>> gameobjects;

		//! textFilter ImGuiTextFilter
		/*! Is used to filter through objects in the hierarchy via the search bar*/
		ImGuiTextFilter textFilter;

	private:

		//! GenerateTree Function
		/*! Generates the gameObject tree for rendering*/
		void GenerateTree(std::shared_ptr<GameObject> go, int index);


		void UpdateActiveObjects();

		//! GetObjIndex Function
		/*! Returns the index of the gameObject with a given name from the gameobjects vector*/
		int GetObjIndex(const std::string& objName) const;

	};

	
	enum class RenderMode : unsigned int
	{
		Result = 0,
		Albedo = 1,
		Position = 2,
		Normals = 3,
		Depth = 4,
		Stencil = 5
		
	};

	enum class AspectRatio : unsigned int
	{
		FREE_ASPECT,
		THREE_TWO,
		FOUR_THREE,
		FIVE_THREE,
		FIVE_FOUR,
		SIXTEEN_NINE,
		SIXTEEN_TEN
	};

	//! Viewport panel Class
	/*!  This class creates & Renders the Viewport Panel UI */
	class Viewport : public UIInterface {


	public:

		Viewport();
		virtual ~Viewport();

		//! Render Function
		/*! Renders all of the supplied IMGui panels */
		void Render() override;


		RenderMode GetRenderMode() const { return renderMode; }

		//! GetIsMouseHovered Function
		/*! Return's the bool isMouseHovered */
		bool GetIsMouseHovered() const { return isMouseHovered; }

		MATH::Vec2 GetViewportSize() const { return viewportSize; }
	
		MATH::Vec2 GetViewportPosition() const { return viewport_Position; }

		int GetMousePosX() const { return MouseX; }
		int GetMousePosY() const { return MouseY; }

	private:
		//! viewportSize Vec2 
		/*! Stores the viewports size */
		MATH::Vec2 viewportSize;
		//! viewport_Min/Max Vec2s 
		/*! Stores the viewport min/max positons on the screen */
		MATH::Vec2 viewport_Min, viewport_Max;

		MATH::Vec2 viewport_Position;

		int MouseX, MouseY;

		std::string modeName;
		std::string aspectSize;

		RenderMode renderMode;
		AspectRatio activeRatio;

		std::vector<const char*> modeMap;
		std::vector<const char*> aspectRatios;

		//! isMouseHovered bool 
		/*! Stores if the mouse is hovering over the viewport or not */
		bool isMouseHovered;

		bool isActive;


		void UpdateViewportPosition();

	};

	

	//! Content Browser Class
	/*!  This class creates & Renders the Content Browser UI */
	class ContentBrowser : public UIInterface
	{
	public:

		ContentBrowser();
		virtual ~ContentBrowser();

		//! Render Function
		/*! Renders all of the supplied IMGui panels */
		void Render() override;

	private:

		//! CurrentDirectory Path
		/*! Stores the current directory the content browser is in */
		std::filesystem::path CurrentDirectory;

		//! DirectoryNames vector
		/*! Stores a broken up list of the current directory e.g. (Objects,Primitives,Cubes) */
		std::vector<std::string> DirectoryNames;

		//! DirectoryItems vector
		/*! Stores all of the items currently in the directory to use for rendering*/
		std::vector<std::filesystem::directory_entry> DirectoryItems;


		//! ItemPadding float
		/*! Stores how much padding each item has in the content browser  */
		float ItemPadding;

		//! ItemSize float
		/*! Stores the size of each item in the content browser */
		float ItemSize;

		bool IsHoveringItem;

		bool QueueForItemMenu;

		std::string SelectedItemName;

	private:

		//! GenerateContent Function
		/*! Generates all the content in the browser*/
		void GenerateContent();

		//! GenerateItem Function
		/*! Generates a specefic item in the content browser*/
		void GenerateItem(const std::filesystem::directory_entry& entry);

		//! GenDirectoryItems Function
		/*! Used to refresh all the items being displayed when the directory is changed*/
		void GenDirectoryItems();

		//! GenDirectoryItems Function
		/*! Used to refresh all the items being displayed when the directory is changed*/
		void GeneratePathNav();

		//! ChangeDirectory Function
		/*! Changes the current directory to whatever directory name is passed in e.g. CurrentDir = (Objects/Primitives/Cubes), PassedInDir = (Primitives), NewDir = (Objects/Primitives)*/
		void ChangeDirectory(const std::string& dir);

	};

	//! Console Log Class
	/*!  This class shows the console output in-editor*/
	class ConsoleLog : public UIInterface
	{
	public:

		ImGuiTextBuffer text;
		ImGuiTextFilter filter;
		ImVector<int> lineOffsets;
		bool scrollToBottom;
		
		ConsoleLog();
		virtual ~ConsoleLog();

		void AddLog(const std::string& message);

		void Render() override;

		void Clear() { text.clear(); lineOffsets.clear(); }
	};
	
	//! DockSpace Class
	/*! This class initializes and renders all of the needed ImGui interfaces*/
	class DockSpace : public UIInterface {

	public:

		DockSpace();

		virtual ~DockSpace();


		//! Update Function
		/*! Updates any of the needed information for the UI to function */
		void Update(const float deltatime);

		//! Render Function
		/*! Renders all of the supplied IMGui panels */
		void Render() override;

		void Reset() override;

		//! ConstructHierarchy Function
		/*! Calls the hierarchy panels Construct Hierarchy function to initialize all of the gameobjects  */
		void ConstructUserInterface();

	private:

		// dockspaceFlags ImGuiDockNodeFlags 
		/*! Used so that we can change the flags the dock space uses throughout more functions */
		ImGuiDockNodeFlags dockspaceFlags;

		// isQueuedForSave bool 
		/*! Return's true if the save button has been pressed */
		bool isQueuedForSave;

		// isDockSpaceOpen bool 
		/*! Return's true if the dock space is opened */
		bool isDockSpaceOpen;

		// isDockSpaceFullScreen bool 
		/*! Return's true if the dock space full screen */
		bool isDockSpaceFullScreen;

		std::vector<UIInterface*> uiInterfaces;

		


	private:

		// GenerateDockSpace function 
		/*! Generates all of the needed IMGUI components of our engine {Content Browser,Viewport e.g.} */
		void GenerateDockSpace();
	};


	

}
#endif




 
