#include "SaveFile.h"
#include "core/Globals.h"
#include "core/CoreEngine.h"
#include "core/scene/Scene.h"

#include <utility>

#include "SaveManager.h"

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)


SaveFile::SaveFile(const std::string& FileName_, const FileType type) : Doc(), rootNode(Doc.NewElement("Root")),
                                                                        insertionOrder(std::vector<std::string>()), Elements(std::map<std::string, ElementInfo>()), HasBeenEdited(false)
{
	FileName = FileName_;

	if (prevFileName.empty())
	{
		prevFileName = FileName;
	}
	fileType = type;

	InitalizeFile();
}



SaveFile::SaveFile(const std::string& FileName_, const std::map<std::string, ElementInfo>& Elements_, tinyxml2::XMLDoc& Doc_) : 
	rootNode(Doc.NewElement("Root")), insertionOrder(std::vector<std::string>()), HasBeenEdited(false)
{
	FileName = FileName_;
	prevFileName = FileName_;
	Elements = Elements_;
	Doc_.DeepCopy(&Doc);

	InitalizeFile();
}

SaveFile::SaveFile() : FileName("None"), Doc(), rootNode(Doc.NewElement("Root")), 
insertionOrder(std::vector<std::string>()), Elements(std::map<std::string, ElementInfo>()), HasBeenEdited(false)
{
	InitalizeFile();
}

SaveFile::SaveFile(const SaveFile& file)
{
	FileName = file.FileName;
	prevFileName = file.prevFileName;
	Elements = file.Elements;
	insertionOrder = file.insertionOrder;
	fileType = file.fileType;
	HasBeenEdited = file.HasBeenEdited;
	file.Doc.DeepCopy(&Doc);
	rootNode = Doc.NewElement("Root");
	FindElement("Root").element = Doc.RootElement();
}



ElementInfo& SaveFile::FindElement(const std::string& elmName)
{
	std::map<std::string, ElementInfo>::iterator iter = Elements.find(elmName);

	if (iter != Elements.end())
	{
		return iter->second;
	}
	else
	{
		EngineLogger::Info(elmName + " Was Not Located When Trying To Be Found", "SaveFile.cpp", __LINE__, MessageTag::TYPE_SAVE);
	}
}

bool SaveFile::HasElement(const std::string& elmName) const
{
	std::map<std::string, ElementInfo>::const_iterator iter = Elements.find(elmName);

	if (iter != Elements.end())
	{
		return true;
	}
	return false;
}

Attribute& SaveFile::FindAttribute(const std::string& elmName, const std::string& atrbName)
{

	std::map<std::string, ElementInfo>::iterator elmIter = Elements.find(elmName);

	if (elmIter != Elements.end())
	{
		std::map<std::string, Attribute>::iterator atribIter = elmIter->second.Attributes.find(atrbName);

		if (atribIter != elmIter->second.Attributes.end())
		{
			return atribIter->second;
		}
		else
		{
			EngineLogger::Warning(atrbName + " Was Not Located When Trying To Be Found", "SaveFile.cpp", __LINE__, MessageTag::TYPE_SAVE);
		}
	}
	else
	{
		EngineLogger::Warning(elmName + " Was Not Located When Trying To Be Found", "SaveFile.cpp", __LINE__, MessageTag::TYPE_SAVE);
	}

}

SaveFile::~SaveFile()
{
	if (!Elements.empty())
	{
		Elements.clear();
	}

	if (!insertionOrder.empty())
	{
		insertionOrder.clear();
	}

	rootNode = nullptr;
}

SaveFile SaveFile::operator=(const SaveFile& file) 
{
	return SaveFile(FileName, Elements, Doc);
}

bool SaveFile::operator==(const SaveFile& file) const
{
	return file.FileName == FileName;
}

void SaveFile::InitalizeFile()
{
	Doc.InsertFirstChild(rootNode);

	ElementInfo info;
	info.element = Doc.RootElement();

	AddElement("Root", info);
	insertionOrder.reserve(10);
}


void SaveFile::AddElement(const std::string& name,  ElementInfo element)
{
	Elements.emplace(name, element);
	insertionOrder.emplace(insertionOrder.end(), name);
}

void SaveFile::AddElements(const std::map<std::string, ElementInfo>& elements)
{
	for (const auto& element : elements)
	{
		Elements.emplace(element.first, element.second);
		insertionOrder.emplace(insertionOrder.end(), element.first);
	}
}

void SaveFile::AddAttribute(const std::string& elmName, const std::string& atribName, Attribute value)
{
	std::map<std::string, ElementInfo>::iterator iter = Elements.begin();

	while (iter != Elements.end())
	{
		if (iter->first == elmName)
		{
			iter->second.Attributes.emplace(atribName, value);
			break;
		}
		iter++;
	}
}



void SaveFile::AddAttributes(const std::string& elmName, const std::map<std::string, Attribute>& attributes)
{
	std::map<std::string, ElementInfo>::iterator iter = Elements.begin();

	while (iter != Elements.end())
	{
		if (iter->first == elmName)
		{	
			for (const auto& Attribs : attributes)
			{
				iter->second.Attributes.emplace(Attribs.first, Attribs.second);
			}
			break;
		}
		iter++;
	}
}

void SaveFile::SetElementName(const std::string& o_ElmName, const std::string& n_ElmName)
{
	std::map<std::string, ElementInfo>::iterator iter = Elements.find(o_ElmName);
	std::vector<std::string>::iterator insertIter = insertionOrder.begin();

	if (iter != Elements.end())
	{
		auto value = std::move(iter->second);
		Elements.erase(iter);
		Elements.insert({ n_ElmName, std::move(value) });
	}

	while (insertIter != insertionOrder.end())
	{
		if (*insertIter == o_ElmName)
		{
			*insertIter = n_ElmName;
			break;
		}
		insertIter++;
	}
}

void SaveFile::RemoveElement(const std::string& name)
{
	std::map<std::string, ElementInfo>::iterator iter = Elements.begin();
	std::vector<std::string>::iterator insertIter = insertionOrder.begin();

	while (iter != Elements.end())
	{
		if (iter->first == name)
		{
			Elements.erase(iter);
			break;
		}
		iter++;
	}

	while (insertIter != insertionOrder.end())
	{
		if (*insertIter == name)
		{
			insertionOrder.erase(insertIter);
			break;
		}
		insertIter++;
	}
}

void SaveFile::ClearElements()
{
	Elements.clear();
	insertionOrder.clear();
}

std::string SaveFile::GetSaveFileType() const
{
	switch (fileType)
	{
	case FileType::DEFAULT:
		return ".xml";

	case FileType::SCENE:
		return ".scene";

	case FileType::OBJECT:
		return ".sbo";
	}

	return ".error";
}

std::string SaveFile::GetFileDestination() const
{
	std::string destination;
	std::string sceneName;

	sceneName = CoreEngine::GetInstance()->GetCurrentScene()->GetSceneName() + "\\";
	destination = Globals::SAVE_DATA_PATH;
	switch (fileType)
	{

	case FileType::DEFAULT:
		destination.append("Default\\");
		break;
	case FileType::SCENE:
		destination.append("Scenes\\");
		break;

	case FileType::OBJECT:
		destination.append("Objects\\");
		destination.append(sceneName);
		break;
	}

	return destination;
}

void SaveFile::Save()
{
	std::string Destination = GetFileDestination();

	SaveFile& save = SaveManager::GetSaveFile(Globals::SCENE_NAME);

	if (save.FileName != save.prevFileName)
	{
		std::filesystem::path oldPath = Globals::SAVE_DATA_PATH + ("Objects\\" + save.prevFileName);
		std::filesystem::path newPath = Globals::SAVE_DATA_PATH + ("Objects\\" + save.FileName);

		if (std::filesystem::exists(oldPath))
		{
			std::filesystem::rename(oldPath, newPath);
		}
	}
	else
	{
		std::filesystem::create_directory(Destination);
	}

	if (FileName != prevFileName)
	{
		std::filesystem::path oldObjPath = Destination + prevFileName + GetSaveFileType();
		std::filesystem::remove(oldObjPath);
	}

	const tinyxml2::XMLError eResult = Doc.SaveFile((Destination + FileName + GetSaveFileType()).c_str());
	if (eResult != tinyxml2::XML_SUCCESS)
	{
		EngineLogger::Error("Unable to save " + FileName + " XML document", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
	}
}
