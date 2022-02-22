#ifndef LOADUTILITY_H
#define LOADUTILITY_H

#include "core/UUniqueID.h"
#include "core/resources/SaveManager.h"


#include <memory>

//! LoadUtility class 
/*! Handle's loading of data from files into SaveFile classes and what then get's done with that data*/
class LoadUtility
{
private:

	static std::unique_ptr<LoadUtility> utilityInstance;
	friend std::default_delete<LoadUtility>;


	//! PrevLoadedObjName string
	/*! Stores the previosly loaded object name via std::get in LoadObject function*/
	std::string S_PrevLoadedObjName;


	//! LoadRecursiveElements function 
	/*! Loops through all elements attached to "element" via GetNextSibling function and add's there information to the savefile*/
	void LoadRecursiveElements(tinyxml2::XMLElement* element, SaveFile& file);
	
	//! LoadSave function 
	/*! Loads up the xml data with the name and path into a new SaveFile object*/
	void LoadSave(const std::string& saveName, const std::string& savePath, FileType extention);

	//! AddObjectToMap function 
	/*! Add's to the SaveAbleObjects unordered_map depending on what class type is passed in*/
	void AddObjectToMap(const char* classType) const;

	//! QueryAtributeValue function 
	/*! Will check what type the XMLAttribute contains and then load it into hte ElementInfo*/
	void QueryAtributeValue(ElementInfo& info, const tinyxml2::XMLAttribute* atrib);

	//! GetFileExtention function 
	/*! Return's the file extention enum that goes with the passed in string*/
	FileType GetFileExtention(const std::string& ext) const;



	
public:

	LoadUtility() = default;
	~LoadUtility() = default;

	static LoadUtility* GetInstance();

	//! LoadExistingSaves function 
	/*! Loops through all of the saves at our SaveFiles filepath and loads them into the engine */
	void LoadExistingSaves();

	//! LoadSceneSaves function 
	/*! Loads the current scenes saves into the engine*/
	void LoadSceneSaves();

	//! UnLoadSceneSaves function 
	/*! Unloads the current scenes saves from the engine*/
	void UnLoadSceneSaves();


	//! LoadObject function 
	/*! Load's a game object into the engine with the information from the savefile*/
	void LoadObject(SaveFile& file);

	//! LoadDefaultScenes function 
	/*! Loads in the non c++ created scenes into the engine*/
	void LoadDefaultScenes(class Game* G_Interface) const;


	template<typename AtribType>
	bool InitalizeAttribute(const std::string Atrib_Name, ElementInfo& Element)
	{
		if (!Element.HasAttribute(Atrib_Name))
		{
			AtribType Atrib = 0;
			Element.Attributes.emplace(Atrib_Name, Atrib);

			return true;
		}

		return false;
	}

};

#endif

