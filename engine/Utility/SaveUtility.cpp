#include "SaveUtility.h"
#include "components/GameObject.h"
#include "core/Globals.h"
#include "core/resources/SaveManager.h"

#include "math/Vector.h"


std::unique_ptr<SaveUtility> SaveUtility::utilityInstance = std::unique_ptr<SaveUtility>();

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

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
	if (SaveManager::SaveFiles.find(saveName) != SaveManager::SaveFiles.end())
	{
		EngineLogger::Warning(saveName + " Already Exists", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
	}
	else
	{
		SaveManager::AddToSaveQueue(saveName, SaveFile(saveName, type));
		EngineLogger::Info(saveName + " Successfully Added To The Save Queue", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
	}

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

		EngineLogger::Warning(saveName + " Was Not Located When Trying To Be Overwritten", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);

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

		EngineLogger::Warning(saveName + " Was Not Located When Trying To Be Overwritten", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);

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

		EngineLogger::Warning(saveName + " Was Not Located When An Element Was Trying To Be Overwritten", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
	}
}

void SaveUtility::AddElement(const std::string saveName, const std::string elmName, const ElementInfo& element)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveFiles.find(saveName);
	std::unordered_map<std::string, SaveFile>::iterator iterQueue = SaveManager::SaveQueue.find(saveName);

	if (iter != SaveManager::SaveFiles.end())
	{
		if (iter->second.HasElement(elmName))
		{
			if (iter->second.FindElement(elmName) != element)
			{
				iter->second.FindElement(elmName) = element;
				iter->second.HasBeenEdited = true;
			}
		}
		else
		{
			iter->second.AddElement(elmName, element);
			iter->second.HasBeenEdited = true;
		}

		SaveManager::TransferToSaveQueue(saveName);

		
	}
	else if (iterQueue != SaveManager::SaveQueue.end())
	{
		if (iterQueue->second.HasElement(elmName))
		{

			if (iterQueue->second.FindElement(elmName) != element)
			{
				iterQueue->second.FindElement(elmName) = element;
				iterQueue->second.HasBeenEdited = true;
			}
			
		}
		else
		{
			iterQueue->second.AddElement(elmName, element);
			iterQueue->second.HasBeenEdited = true;

		}
	}
	else {



		EngineLogger::Warning(saveName + " Was Not Located When Trying To Add An Element", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
	}
}

void SaveUtility::AddElement(const std::string saveName, const std::string elmName, const std::string parentName, tinyxml2::XMLElement* element)
{
	ElementInfo elmInfo = ElementInfo();
	elmInfo.element = element;
	elmInfo.parentName = parentName;

	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveFiles.find(saveName);
	std::unordered_map<std::string, SaveFile>::iterator iterQueue = SaveManager::SaveQueue.find(saveName);

	if (iter != SaveManager::SaveFiles.end())
	{
		if (iter->second.HasElement(elmName))
		{
			iter->second.FindElement(elmName) = elmInfo;
		}
		else
		{
			iter->second.AddElement(elmName, elmInfo);
		}

		SaveManager::TransferToSaveQueue(saveName);


	}
	else if (iterQueue != SaveManager::SaveQueue.end())
	{
		if (iterQueue->second.HasElement(elmName))
		{
			iterQueue->second.FindElement(elmName) = elmInfo;
		}
		else
		{
			iterQueue->second.AddElement(elmName, elmInfo);
		}
	}
	else {



		EngineLogger::Warning(saveName + " Was Not Located When Trying To Add An Element", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
	}
}



void SaveUtility::RemoveElement(const std::string saveName, const std::string elmName)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveFiles.find(saveName);
	std::unordered_map<std::string, SaveFile>::iterator iterQueue = SaveManager::SaveQueue.find(saveName);

	if (iter != SaveManager::SaveFiles.end())
	{
		if (iter->second.HasElement(elmName))
		{
			iter->second.RemoveElement(elmName);
		}

	}
	else if (iterQueue != SaveManager::SaveQueue.end())
	{
		if (iterQueue->second.HasElement(elmName))
		{
			iterQueue->second.RemoveElement(elmName);
		}
	}

}

void SaveUtility::RemoveAllElements(const std::string saveName)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveManager::SaveFiles.find(saveName);
	std::unordered_map<std::string, SaveFile>::iterator iterQueue = SaveManager::SaveQueue.find(saveName);

	if (iter != SaveManager::SaveFiles.end())
	{

		
		iter->second.ClearElements();

	}
	else if (iterQueue != SaveManager::SaveQueue.end())
	{
		iterQueue->second.ClearElements();
	}
}

ElementInfo SaveUtility::CreateVec3(const MATH::Vec3& value, std::string parentName)
{

	ElementInfo element = ElementInfo(parentName);
	for (int i = 0; i < 3; i++)
	{
		element.Attributes.emplace(Globals::IntToVector(i), value[i]);
	}

	return element;
}

ElementInfo SaveUtility::CreateVec4(const MATH::Vec4& value, std::string parentName)
{
	ElementInfo element = ElementInfo(parentName);
	for (int i = 0; i < 4; i++)
	{
		element.Attributes.emplace(Globals::IntToVector(i), value[i]);
	}

	return element;
}

void SaveUtility::CompileSaves()
{

	EngineLogger::Info("===========SAVES BEING COMPILED===========", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
	for (auto& save : SaveManager::SaveQueue)
	{
		if(!save.second.HasBeenEdited)
			continue;

		for (auto& elmName : save.second.insertionOrder)
		{
			ElementInfo &elm = save.second.Elements[elmName];
			if (elm.IsRootChild())
			{
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

		EngineLogger::Info(save.first + " Compiled Successfully", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
	}

	EngineLogger::Info("===========SAVES FINISHED COMPILING===========", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);


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



	AddElement(saveName, "Position", CreateVec3(obj->transform.pos,"Transform"));
	AddElement(saveName, "Rotation", CreateVec3(MATH::Quaternion::QuatToEuler(obj->transform.rotation), "Transform"));
	AddElement(saveName, "Scale", CreateVec3(obj->transform.scale, "Transform"));


	ElementInfo ObjectInfo = ElementInfo("Name");
	AddElement(saveName, "ObjectInfo", ObjectInfo);


	ElementInfo ObjectType = ElementInfo("ObjectInfo");
	ObjectType.Attributes.emplace("ID", std::string(obj->GetType()));
	AddElement(saveName, "Type", ObjectType);

	ElementInfo Components = ElementInfo("Name");
	AddElement(saveName, "Components", Components);

	for (auto comp : obj->GetComponents())
	{
		comp->OnSaveComponent(saveName,"Components");
	}


}