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



	void LoadRecersiveElements(tinyxml2::XMLElement* element, SaveFile& file);

	void AddObjectToMap(const char* classType) const;

	void LoadSave(const std::string saveName, const std::string savePath,FileType extention);

	void QueryAtributeValue(ElementInfo& info, const tinyxml2::XMLAttribute* atrib);

	FileType GetFileExtention(std::string ext) const;
public:

	LoadUtility();
	~LoadUtility();

	static LoadUtility* GetInstance();


	void LoadExistingSaves();

	void LoadSceneSaves(std::string& objPath);

	void LoadObject(SaveFile& file);




	int LoadInt(std::string saveName, std::string elmName, std::string atribName);
	
	float LoadFloat(std::string saveName, std::string elmName, std::string atribName);

	std::string LoadString(std::string saveName, std::string elmName, std::string atribName);

};

#endif

