#ifndef LOADUTILITY_H
#define LOADUTILITY_H

#include <memory>

#include "core//resources/SaveManager.h"


class LoadUtility
{
private:

	static std::unique_ptr<LoadUtility> utilityInstance;
	friend std::default_delete<LoadUtility>;

	std::string prevLoadedObjName;



	void LoadRecursiveElements(tinyxml2::XMLElement* element, SaveFile& file);
	void AddObjectToMap(const char* classType) const;

	void LoadSave(const std::string& saveName, const std::string& savePath, FileType extention);

	void QueryAtributeValue(ElementInfo& info, const tinyxml2::XMLAttribute* atrib);

	FileType GetFileExtention(const std::string& ext) const;
public:

	LoadUtility() = default;
	~LoadUtility() = default;

	static LoadUtility* GetInstance();

	void LoadExistingSaves();

	void LoadSceneSaves();
	void UnLoadSceneSaves();

	void LoadObject(SaveFile& file);

	void LoadDefaultScenes(class GameInterface* G_Interface) const;

	int LoadInt(const std::string& saveName, const std::string& elmName, const std::string& atribName);
	
	float LoadFloat(const std::string& saveName, const std::string& elmName, const std::string& atribName);

	std::string LoadString(const std::string& saveName, const  std::string& elmName, const std::string& atribName);

};

#endif

