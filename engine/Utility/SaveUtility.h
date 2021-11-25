
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


	void CreateSave(const std::string& saveName,FileType type = FileType::DEFAULT);
	void CreateSave(const std::string& saveName, const std::map<std::string, ElementInfo>& elements);
	void CreateSave(const std::string& saveName,const SaveFile& save);

	void DeleteSave(const std::string& saveName);

	void OverwriteSave(const std::string& saveName, const SaveFile& save);
	void OverwriteSave(const SaveFile& oldSave, const SaveFile& newSave);
	void OverwriteSave(const std::string& saveName, const std::map<std::string, ElementInfo>& elements);

	void OverwriteElement(const std::string& saveName, const std::string& elmName, const ElementInfo& element);

	void AddElement(const std::string& saveName, const std::string& elmName, const ElementInfo& element);
	void AddElement(const std::string& saveName, const std::string& elmName, const std::string& parentName, tinyxml2::XMLElement* element = nullptr);


	void RemoveElement(const std::string& saveName, const std::string& elmName);
	void RemoveAllElements(const std::string& saveName);


	ElementInfo CreateVec3(const MATH::Vec3& value,const std::string& parentName);

	ElementInfo CreateVec4(const MATH::Vec4& value, const std::string& parentName);

	void SaveObject(const std::string& saveName, GameObject* obj);
};

#endif