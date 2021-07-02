#include "SaveFile.h"
#include "core/Globals.h"




SaveFile::SaveFile(const std::string& FileName_, const FileType type) : Doc(), Elements(std::map<std::string, ElementInfo>()), rootNode(Doc.NewElement("Root")), insertionOrder(std::vector<std::string>())
{
	FileName = FileName_;
	fileType = type;


	InitalizeFile();
}

SaveFile::SaveFile(std::string& FileName_, std::map<std::string, ElementInfo> Elements_, tinyxml2::XMLDoc& Doc_) : rootNode(Doc.NewElement("Root")), insertionOrder(std::vector<std::string>())
{
	FileName = FileName_;
	Elements = Elements_;
	Doc_.DeepCopy(&Doc);

	InitalizeFile();

	
}

SaveFile::SaveFile() : Doc(), Elements(std::map<std::string, ElementInfo>()), FileName("None"), rootNode(Doc.NewElement("Root")), insertionOrder(std::vector<std::string>())
{
	InitalizeFile();
}

SaveFile::SaveFile(const SaveFile& file)
{
	FileName = file.FileName;
	Elements = file.Elements;
	insertionOrder = file.insertionOrder;
	fileType = file.fileType;
	file.Doc.DeepCopy(&Doc);
	rootNode = Doc.NewElement("Root");
}

ElementInfo& SaveFile::FindElement(std::string elmName)
{
	std::map<std::string, ElementInfo>::iterator iter = Elements.find(elmName);

	if (iter != Elements.end())
	{
		return iter->second;
	}
	else
	{
		EngineLogger::Info(elmName + " Was Not Located When Trying To Be Found", "SaveFile.cpp", __LINE__);
	}
}

SaveFile::~SaveFile()
{
	if (Elements.size() > 0)
	{
		Elements.clear();
	}

	if (insertionOrder.size() > 0)
	{
		insertionOrder.clear();
	}

	rootNode = nullptr;
}

SaveFile SaveFile::operator=(const SaveFile& file) 
{
	return SaveFile(FileName, Elements, Doc);
}

bool SaveFile::operator==(const SaveFile& file)
{
	return file.FileName == FileName;
}

void SaveFile::InitalizeFile()
{
	Doc.InsertFirstChild(rootNode);

	ElementInfo info;
	info.element = Doc.FirstChildElement("Root");

	AddElement("Root", info);
	insertionOrder.reserve(10);
}


void SaveFile::AddElement(const std::string name,  ElementInfo element)
{
	Elements.emplace(name, element);
	insertionOrder.emplace(insertionOrder.end(), name);
}

void SaveFile::AddElements(const std::map<std::string, ElementInfo> elements)
{
	for (auto element : elements)
	{
		Elements.emplace(element.first, element.second);
	}
}

void SaveFile::AddAttribute(const std::string elmName, const std::string atribName, Attribute value)
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



void SaveFile::AddAttributes(const std::string elmName, std::map<std::string, Attribute> attributes)
{
	std::map<std::string, ElementInfo>::iterator iter = Elements.begin();

	while (iter != Elements.end())
	{

		if (iter->first == elmName)
		{
			
			for (auto Attribs : attributes)
			{
				iter->second.Attributes.emplace(Attribs.first, Attribs.second);
			}
			break;
		}

		iter++;
	}
}

std::string SaveFile::GetFileType()
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

std::string SaveFile::GetFileDestination()
{
	std::string destination;

	destination = Globals::ENGINE_PATH;
	switch (fileType)
	{

	case FileType::DEFAULT:
		destination.append("\\resources\\SaveData\\Default\\");
		break;
	case FileType::SCENE:
		destination.append("\\resources\\SaveData\\Scenes\\");
		break;

	case FileType::OBJECT:
		destination.append("\\resources\\SaveData\\Objects\\");
		break;
	}

	return destination;
}

void SaveFile::Save()
{
	const tinyxml2::XMLError eResult = Doc.SaveFile((GetFileDestination() + FileName + GetFileType()).c_str());
	if (eResult != tinyxml2::XML_SUCCESS)
	{
		EngineLogger::Error("Unable to save " + FileName + " XML document", "SaveUtility.cpp", __LINE__);
	}
}
