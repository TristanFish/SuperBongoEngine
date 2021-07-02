#include "LoadUtility.h"

std::unique_ptr<LoadUtility> LoadUtility::utilityInstance = std::unique_ptr<LoadUtility>();



LoadUtility::LoadUtility()
{

}

LoadUtility::~LoadUtility()
{

}

LoadUtility* LoadUtility::GetInstance()
{
	if (utilityInstance == nullptr)
	{
		utilityInstance.reset(new LoadUtility);
	}
	return utilityInstance.get();
}

void LoadUtility::LoadExistingSave(std::string fileName)
{
	tinyxml2::XMLDoc SaveData;
	tinyxml2::XMLError eResult = SaveData.LoadFile((fileName + ".xml").c_str());

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		EngineLogger::Error("Couldn't load the Save File " + fileName, "LoadUtility.cpp", __LINE__);
	}

	tinyxml2::XMLNode* pRoot = SaveData.FirstChild();


}