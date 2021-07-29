
#ifndef SAVEUTILITY_H
#define SAVEUTILITY_H

#include <memory>

#include "core/resources/SaveFile.h"

class GameObject;
class SaveUtility
{
private:

	static std::unique_ptr<SaveUtility> utilityInstance;
	friend std::default_delete<SaveUtility>;




	void HandleAttributes(SaveFile& save,ElementInfo& elm);
public:



	SaveUtility();
	~SaveUtility();

	static SaveUtility* GetInstance();

	void CompileSaves();


	void CreateSave(const std::string saveName,FileType type = FileType::DEFAULT);
	void CreateSave(const std::string saveName, const std::map<std::string, ElementInfo>& elements);
	void CreateSave(const std::string saveName,const SaveFile& save);

	void DeleteSave(const std::string saveName);

	void OverwriteSave(const std::string saveName, const SaveFile& save);
	void OverwriteSave(const SaveFile& oldSave, const SaveFile& newSave);
	void OverwriteSave(const std::string saveName, const std::map<std::string, ElementInfo>& elements);

	void OverwriteElement(const std::string saveName, const std::string elmName, const ElementInfo& element);

	void AddElement(const std::string saveName, const std::string elmName, const ElementInfo& element);

	void AddElement(const std::string saveName, const std::string elmName, const std::string parentName, tinyxml2::XMLElement* element = nullptr);


	void SaveObject(const std::string saveName, GameObject* obj);
};

#endif