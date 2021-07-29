#include "SaveUtility.h"
#include "components/GameObject.h"
#include "core/Globals.h"
#include "core/resources/SaveManager.h"
std::unique_ptr<SaveUtility> SaveUtility::utilityInstance = std::unique_ptr<SaveUtility>();




void SaveUtility::HandleAttributes(SaveFile& save, ElementInfo& elm)
{
	for (auto& atrib : elm.Attributes)
	{
		if (std::holds_alternative<int>(atrib.second))
			elm.element->SetAttribute(atrib.first.c_str(), std::get<int>(atrib.second));

		else if (std::holds_alternative<float>(atrib.second))
			elm.element->SetAttribute(atrib.first.c_str(), std::get<float>(atrib.second));

		else if (std::holds_alternative<double>(atrib.second))
			elm.element->SetAttribute(atrib.first.c_str(), std::get<float>(atrib.second));

		else if (std::holds_alternative<std::string>(atrib.second))
			elm.element->SetAttribute(atrib.first.c_str(), std::get<std::string>(atrib.second).c_str());

		else if (std::holds_alternative<bool>(atrib.second))
			elm.element->SetAttribute(atrib.first.c_str(), std::get<bool>(atrib.second));

		save.FindElement(elm.parentName).element->InsertEndChild(elm.element);
	}
}

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
	EngineLogger::Save(saveName + " Successfully Added To The Save Queue", "SaveUtility.cpp", __LINE__);

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

		EngineLogger::Save(saveName + " Was Not Located When Trying To Be Overwritten", "SaveUtility.cpp", __LINE__);

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

		EngineLogger::Save(saveName + " Was Not Located When Trying To Be Overwritten", "SaveUtility.cpp", __LINE__);

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

		EngineLogger::Save(saveName + " Was Not Located When An Element Was Trying To Be Overwritten", "SaveUtility.cpp", __LINE__);
	}
}

void SaveUtility::AddElement(const std::string saveName, const std::string elmName, const ElementInfo& element)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveQueue.find(saveName);

	if (iter != SaveManager::SaveQueue.end())
	{
		iter->second.AddElement(elmName, element);
	}
	else {

		EngineLogger::Save(saveName + " Was Not Located When Trying To Add An Element", "SaveUtility.cpp", __LINE__);
	}
}

void SaveUtility::AddElement(const std::string saveName, const std::string elmName, const std::string parentName, tinyxml2::XMLElement* element)
{
	ElementInfo elmInfo = ElementInfo();

	elmInfo.element = element;
	elmInfo.parentName = parentName;
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveQueue.find(saveName);

	if (iter != SaveManager::SaveQueue.end())
	{
		iter->second.AddElement(elmName, elmInfo);
	}
	else {

		EngineLogger::Save(saveName + " Was Not Located When Trying To Add An Element", "SaveUtility.cpp", __LINE__);
	}
}



void SaveUtility::CompileSaves()
{

	EngineLogger::Save("===========SAVES BEING COMPILED===========", "SaveUtility.cpp", __LINE__);
	for (auto& save : SaveManager::SaveQueue)
	{
		for (auto& elmName : save.second.insertionOrder)
		{
			ElementInfo &elm = save.second.Elements[elmName];
			if (elm.IsRootChild())
			{
				//save.second.rootNode = save.second.Doc.NewElement("Root");
				elm.element = save.second.Doc.NewElement(elmName.c_str());
				save.second.rootNode->InsertFirstChild(elm.element);

				HandleAttributes(save.second, elm);
				
			}

			else if (elmName != "Root")
			{
				elm.element = save.second.Doc.NewElement(elmName.c_str());

				HandleAttributes(save.second, elm);

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

		EngineLogger::Save(save.first + " Compiled Successfully", "SaveUtility.cpp", __LINE__);
	}

	EngineLogger::Save("===========SAVES FINISHED COMPILING===========", "SaveUtility.cpp", __LINE__);


	SaveManager::SaveAll();
}

void SaveUtility::SaveObject(const std::string saveName, GameObject* obj)
{
	CreateSave(saveName, FileType::OBJECT);

	ElementInfo Name = ElementInfo("Root");
	Name.Attributes.emplace("Is", std::string(obj->name));
	AddElement(saveName, "Name", Name);



	ElementInfo Transform = ElementInfo("Name");
	AddElement(saveName, "Transform", Transform);


	ElementInfo Position = ElementInfo("Transform");
	ElementInfo Rotation = ElementInfo("Transform");
	ElementInfo Scale = ElementInfo("Transform");


	for (int i = 0; i < 3; i++)
	{
		Position.Attributes.emplace(Globals::IntToVec3(i), obj->transform.pos[i]);
		Rotation.Attributes.emplace(Globals::IntToVec3(i), obj->transform.rotation[i]);
		Scale.Attributes.emplace(Globals::IntToVec3(i), obj->transform.scale[i]);

	}
	AddElement(saveName, "Position", Position);
	AddElement(saveName, "Rotation", Rotation);
	AddElement(saveName, "Scale", Scale);


	ElementInfo ObjectInfo = ElementInfo("Name");
	AddElement(saveName, "ObjectInfo", ObjectInfo);


	ElementInfo ObjectType = ElementInfo("ObjectInfo");
	ObjectType.Attributes.emplace("ID", std::string(obj->GetType()));
	AddElement(saveName, "Type", ObjectType);
}