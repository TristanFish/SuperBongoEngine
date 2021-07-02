#include "SaveUtility.h"
#include "core/resources/SaveManager.h"
std::unique_ptr<SaveUtility> SaveUtility::utilityInstance = std::unique_ptr<SaveUtility>();


SaveUtility::SaveUtility()
{

}

SaveUtility::~SaveUtility()
{

}

SaveUtility* SaveUtility::GetInstance()
{
	if (utilityInstance == nullptr)
	{
		utilityInstance.reset(new SaveUtility);
	}
	return utilityInstance.get();
}


void SaveUtility::CreateSave(const std::string saveName, FileType type)
{
	SaveManager::AddToSaveQueue(saveName, SaveFile(saveName,type));
	EngineLogger::Info(saveName + " Successfully Added To The Save Queue", "SaveUtility.cpp", __LINE__);

}

void SaveUtility::CreateSave(const std::string saveName, const std::map<std::string, ElementInfo>& elements)
{
	SaveFile tempFile = SaveFile(saveName);

	tempFile.Elements = elements;

	SaveManager::AddToSaveQueue(saveName, tempFile);
}

void SaveUtility::CreateSave(const std::string saveName, const SaveFile& save)
{
	SaveManager::AddToSaveQueue(saveName, save);

}

void SaveUtility::DeleteSave(const std::string saveName)
{
	SaveManager::RemoveSave(saveName);
}

void SaveUtility::OverwriteSave(const std::string saveName, const SaveFile& save)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveFiles.find(saveName);

	if (iter != SaveManager::SaveFiles.end())
	{
		iter->second = save;
	}
	else {

		EngineLogger::Info(saveName + " Was Not Located When Trying To Be Overwritten", "SaveUtility.cpp", __LINE__);

	}
}

void SaveUtility::OverwriteSave(const SaveFile& oldSave, const SaveFile& newSave)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveFiles.begin();

	while (iter != SaveManager::SaveFiles.end())
	{
		if (iter->second == oldSave)
		{
			iter->second = newSave;
		}
		iter++;
	}

}

void SaveUtility::OverwriteSave(const std::string saveName, const std::map<std::string, ElementInfo>& elements)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveFiles.find(saveName);

	if (iter != SaveManager::SaveFiles.end())
	{
		iter->second.Elements = elements;
	}
	else {

		EngineLogger::Info(saveName + " Was Not Located When Trying To Be Overwritten", "SaveUtility.cpp", __LINE__);

	}
}

void SaveUtility::OverwriteElement(const std::string saveName, const std::string elmName, const ElementInfo& element)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveFiles.find(saveName);

	if (iter != SaveManager::SaveFiles.end())
	{
		iter->second.FindElement(elmName) = element;
	}
	else {

		EngineLogger::Info(saveName + " Was Not Located When An Element Was Trying To Be Overwritten", "SaveUtility.cpp", __LINE__);
	}
}

void SaveUtility::AddElement(const std::string saveName, const std::string elmName, const ElementInfo& element)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveFiles.find(saveName);

	if (iter != SaveManager::SaveFiles.end())
	{
		iter->second.AddElement(elmName, element);
	}
	else {

		EngineLogger::Info(saveName + " Was Not Located When Trying To Add An Element", "SaveUtility.cpp", __LINE__);
	}
}

void SaveUtility::AddElement(const std::string saveName, const std::string elmName, const std::string parentName, tinyxml2::XMLElement* element)
{
	ElementInfo elmInfo = ElementInfo();

	elmInfo.element = element;
	elmInfo.parentName = parentName;
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveFiles.find(saveName);

	if (iter != SaveManager::SaveFiles.end())
	{
		iter->second.AddElement(elmName, elmInfo);
	}
	else {

		EngineLogger::Info(saveName + " Was Not Located When Trying To Add An Element", "SaveUtility.cpp", __LINE__);
	}
}

void SaveUtility::CompileSaves()
{
	for (auto& save : SaveManager::SaveQueue)
	{
		for (auto& elmName : save.second.insertionOrder)
		{
			ElementInfo elm = save.second.Elements[elmName];
			if (elm.IsRootChild())
			{
				//save.second.rootNode = save.second.Doc.NewElement("Root");
				elm.element = save.second.Doc.NewElement(elmName.c_str());
				save.second.rootNode->InsertFirstChild(elm.element);
			}

			else if (elmName != "Root")
			{
				elm.element = save.second.Doc.NewElement(elmName.c_str());

				for (auto& atrib : elm.Attributes)
				{
					elm.element->SetAttribute(atrib.first.c_str(), atrib.second.index());
					save.second.FindElement(elm.parentName).element->InsertEndChild(elm.element);
				}


				save.second.FindElement(elm.parentName).element->InsertEndChild(elm.element);
			}
		}

		for (auto& elm : save.second.Elements)
		{
			if (elm.second.IsRootChild())
			{
				save.second.FindElement(elm.second.parentName).element->InsertEndChild(elm.second.element);
			}
		}

		save.second.Save();
	}

	SaveManager::SaveAll();
}
