
#ifndef SAVEUTILITY_H
#define SAVEUTILITY_H

#include <memory>

#include "core/resources/SaveFile.h"
#include "math/Vector.h"

class GameObject;

//! SaveUtility class
/*! Singleton that allows changes to be made to save files and the ability to transfer those savefiles to xml*/
class SaveUtility
{
private:

	static std::unique_ptr<SaveUtility> utilityInstance;
	friend std::default_delete<SaveUtility>;



	//! HandleAttributes function
	/*! Loops through the elements attributes and makes sure to initialize it with the right name and values */
	void HandleAttributes(SaveFile& save,const ElementInfo& elm);
public:



	SaveUtility();
	~SaveUtility();

	static SaveUtility* GetInstance();

	//! CompileSaves function
	/*! Loops through all of the Key/Value pairs in SaveQueue, sets all of the tinyxml2 types, saves the file to a specific path */
	void CompileSaves();

	//! CompileSaves function
	/*! Loops through all of the Key/Value pairs in SaveQueue, sets all of the tinyxml2 types, saves the file to a specific path */
	void CompileSave(SaveFile& SaveFile_, std::string DirectoryOverride = std::string());

	//! CreateSave function 
	/*! Creates a save file with the given name and file type */
	void CreateSave(const std::string& saveName,FileType type = FileType::DEFAULT);

	//! CreateSave function overload 1
	/*! Creates a save file with the given name, elements map and filetype*/
	void CreateSave(const std::string& saveName, const std::map<std::string, ElementInfo>& elements, FileType type = FileType::DEFAULT);
	
	//! CreateSave function overload 2 
	/*! Creates a save file with the given name and savefile ref*/
	void CreateSave(const std::string& saveName,const SaveFile& save);

	//! DeleteSave function 
	/*! Deletes the save with a given name*/
	void DeleteSave(const std::string& saveName);

	//! OverwriteSave function 
	/*! Overwrites a save with the given name with the passed in save*/
	void OverwriteSave(const std::string& saveName, const SaveFile& save);

	//! OverwriteSave function overload 1
	/*! Overwrites the oldSave with the newSave*/
	void OverwriteSave(const SaveFile& oldSave, const SaveFile& newSave);

	//! OverwriteSave function overload 2
	/*! Overwrites a saves elements*/
	void OverwriteSave(const std::string& saveName, const std::map<std::string, ElementInfo>& elements);

	//! OverwriteSave function overload 2
	/*! Overwrites a saves elements*/
	void OverwriteElement(const std::string& saveName, const std::string& elmName, const ElementInfo& element);

	//! AddElement function 
	/*! Add's an element to a specific save with the element name and refrence to Info struct*/
	void AddElement(const std::string& saveName, const std::string& elmName, const ElementInfo& element);

	//! AddElement function overload 1
	/*! Add's an element to a specific save with the element name, parentName and XMLElement pointer*/
	void AddElement(const std::string& saveName, const std::string& elmName, const std::string& parentName, tinyxml2::XMLElement* element = nullptr);

	//! RemoveElement function 
	/*! Removes an element from a specific save with the element name*/
	void RemoveElement(const std::string& saveName, const std::string& elmName);

	//! RemoveAllElements function 
	/*! Removes all the elements in the given save file*/
	void RemoveAllElements(const std::string& saveName);

	//! CreateVec3 function 
	/*! Returns an ElementInfo with all Vec3 values as attributes*/
	ElementInfo CreateVec3(const MATH::Vec3& value,const std::string& parentName,bool UsePrefix = true);

	//! CreateVec4 function 
	/*! Returns an ElementInfo with all Vec4 values as attributes*/
	ElementInfo CreateVec4(const MATH::Vec4& value, const std::string& parentName, bool UsePrefix = true);


	//! SaveObject function 
	/*! Save's all of a gameobjects information as ElementInfo's and adds them to the save file*/
	void SaveObject(const std::string& saveName, std::shared_ptr<GameObject> obj);
};

#endif