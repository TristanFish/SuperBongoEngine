#include "LoadUtility.h"
#include "core/Globals.h"
#include "../game/gameObjects/Bird.h"
#include "../game/gameObjects/Grass.h"
#include "../game/gameObjects/LightObject.h"
#include "../game/gameObjects/Player.h"
#include "../game/gameObjects/TestModel.h"

#include "core/CoreEngine.h"
#include "scenes/Scene.h"

#include "Primitives/Primitives.h"
#include <filesystem>


std::unique_ptr<LoadUtility> LoadUtility::utilityInstance = std::unique_ptr<LoadUtility>();


LoadUtility::LoadUtility()
{

}

LoadUtility::~LoadUtility()
{

}



void LoadUtility::LoadRecersiveElements(tinyxml2::XMLElement* element, SaveFile& file)
{


	for (tinyxml2::XMLElement* Elem = element; Elem != nullptr; Elem = Elem->NextSiblingElement())
	{

		tinyxml2::XMLElement* tmpElement = Elem;
		// LOOP THROUGH NEXT ELEMENT ALSO
		ElementInfo info;
		info.parentName = Elem->Parent()->Value();

		for (const tinyxml2::XMLAttribute* atrib = Elem->FirstAttribute(); atrib != nullptr; atrib = atrib->Next())
		{
			QueryAtributeValue(info, atrib);
		}

		file.AddElement(Elem->Value(), info);

		if (!tmpElement->NoChildren())
		{
			LoadRecersiveElements(tmpElement->FirstChildElement(), file);
		}
	}
}

void LoadUtility::AddObjectToMap(const char* classType) const
{

	if (classType == std::string("class Bird"))
	{
		SaveManager::SaveableObjects.emplace(classType,new Bird("None", MATH::Vec3()));
	}
	else if (classType == std::string("class Grass"))
	{
		SaveManager::SaveableObjects.emplace(classType, new Grass("None", MATH::Vec3(),10));
	}
	else if (classType == std::string("class LightObject"))
	{
		SaveManager::SaveableObjects.emplace(classType, new LightObject("None", MATH::Vec3()));
	}
	else if (classType == std::string("class Player"))
	{
		SaveManager::SaveableObjects.emplace(classType, new Player("None", MATH::Vec3()));
	}
	else if (classType == std::string("class TestModel"))
	{
		SaveManager::SaveableObjects.emplace(classType, new TestModel("None", MATH::Vec3()));
	}
	else if (classType == std::string("class Box"))
	{
		SaveManager::SaveableObjects.emplace(classType, new Box("None", MATH::Vec3()));
	}
	else if (classType == std::string("class PlaneObject"))
	{
		SaveManager::SaveableObjects.emplace(classType, new PlaneObject("None", MATH::Vec3()));
	}
	else if (classType == std::string("class Sphere"))
	{
		SaveManager::SaveableObjects.emplace(classType, new Sphere("None", MATH::Vec3()));
	}

	else {

		EngineLogger::Error("Object could not be added to loadable objects map" , "LoadUtility.cpp", __LINE__);
		std::cout << classType << std::endl;
	}
}

void LoadUtility::LoadSave(const std::string saveName,  std::string savePath, FileType extention)
{
	SaveFile file = SaveFile(saveName, extention);

	tinyxml2::XMLDoc SaveData;

	tinyxml2::XMLError eResult = SaveData.LoadFile(savePath.c_str());


	

	if (eResult != tinyxml2::XML_SUCCESS)
	{
		EngineLogger::Error("Couldn't load the Save File " + saveName, "LoadUtility.cpp", __LINE__);
	}

	tinyxml2::XMLNode* pRoot = SaveData.FirstChildElement("Root");


	LoadRecersiveElements(pRoot->FirstChildElement(), file);
	


	if (extention == FileType::OBJECT)
	{
		std::string classType;
		classType = std::get<std::string>(file.FindAttribute("Type", "ID"));



		if (SaveManager::SaveableObjects.find(classType.c_str()) == SaveManager::SaveableObjects.end())
		{

			AddObjectToMap(classType.c_str());
		}
	}

	if (extention == FileType::SCENE)
	{
		file.SetHasBeenEdited(true);
	}


	SaveManager::AddToSaveFiles(saveName,SaveFile(file));
}

void LoadUtility::QueryAtributeValue(ElementInfo& info, const tinyxml2::XMLAttribute* atrib)
{
	bool boolValue = atrib->BoolValue();

	int intValue = atrib->IntValue();

	float floatValue = atrib->FloatValue();


	 if (atrib->QueryFloatValue(&floatValue) == tinyxml2::XMLError::XML_SUCCESS)
	{
	info.Attributes.emplace(atrib->Name(), floatValue);
	}

	else if (atrib->QueryIntValue(&intValue) == tinyxml2::XMLError::XML_SUCCESS)
	{
		info.Attributes.emplace(atrib->Name(), intValue);
	}

	else if (atrib->Value() != "")
	 {
		 info.Attributes.emplace(atrib->Name(), std::string(atrib->Value()));
	 }

	else if (atrib->QueryBoolValue(&boolValue) == tinyxml2::XMLError::XML_SUCCESS)
	 {
		 info.Attributes.emplace(atrib->Name(), boolValue);
	 }
	else
	{
		EngineLogger::Info("Attribute type unable to be queried", "LoadUtility.cpp", __LINE__);
	}
}

FileType LoadUtility::GetFileExtention( std::string ext) const
{

	if (ext == ".scene")
	{
		return FileType::SCENE;
	}
	else if (ext == ".sbo")
	{
		return FileType::OBJECT;
	}
	else 
	{
		return FileType::DEFAULT;
	}
}



LoadUtility* LoadUtility::GetInstance()
{
	if (utilityInstance == nullptr)
	{
		utilityInstance.reset(new LoadUtility);
	}
	return utilityInstance.get();
}

void LoadUtility::LoadExistingSaves()
{
	std::string directory = Globals::SAVE_DATA_PATH;

	std::string objDir = "Objects\\";

	std::string sceneObjPath = (directory + objDir) + CoreEngine::GetInstance()->GetCurrentScene()->GetSceneName() + "\\";




	EngineLogger::Save("===========EXISTING SAVES BEING LOADED===========", "SaveUtility.cpp", __LINE__);

	for (auto entry = std::filesystem::recursive_directory_iterator(directory); entry != std::filesystem::recursive_directory_iterator(); ++entry)
	{

		std::filesystem::path curPath = entry->path();

		if(curPath == sceneObjPath + curPath.filename().string())
			continue;
		if (entry->is_regular_file())
		{			
			LoadSave(curPath.stem().string(), curPath.string(), GetFileExtention(curPath.extension().string()));
			
		}

	}

	LoadSceneSaves(sceneObjPath);


	EngineLogger::Save("===========EXISTING SAVES SUCCESFULLY LOADED===========", "SaveUtility.cpp", __LINE__);
}

void LoadUtility::LoadSceneSaves(std::string& objPath)
{
	for (auto entry = std::filesystem::directory_iterator(objPath); entry != std::filesystem::directory_iterator(); ++entry)
	{
		if (entry->is_regular_file())
		{
			std::filesystem::path curPath = entry->path();


			LoadSave(curPath.stem().string(), curPath.string(), GetFileExtention(curPath.extension().string()));
		}
	}
}

int LoadUtility::LoadInt(std::string saveName, std::string elmName, std::string atribName)
{
	int QueryiedValue = 0;
	SaveManager::GetSaveFile(saveName).FindElement(elmName).element->QueryIntAttribute(atribName.c_str(), &QueryiedValue);

	return QueryiedValue;
}

float LoadUtility::LoadFloat(std::string saveName, std::string elmName, std::string atribName)
{
	float QueryiedValue = 0.0f;
	SaveManager::GetSaveFile(saveName).FindElement(elmName).element->QueryFloatAttribute(atribName.c_str(), &QueryiedValue);

	return QueryiedValue;
}

void LoadUtility::LoadObject(SaveFile& file)
{
	if (file.GetFileType() == FileType::OBJECT)
	{
		ElementInfo PosElm = file.FindElement("Position");
		ElementInfo RotElm = file.FindElement("Rotation");
		ElementInfo ScaleElm = file.FindElement("Scale");
		ElementInfo TypeElm = file.FindElement("Type");
		ElementInfo NameElm = file.FindElement("Name");

		MATH::Vec3 Position;
		MATH::Vec3 Rotation;
		MATH::Vec3 Scale;

		for (int i = 0; i < 3; i++)
		{

			Position[i] = std::get<float>(PosElm.Attributes[Globals::IntToVector(i)]);
			Rotation[i] = std::get<float>(RotElm.Attributes[Globals::IntToVector(i)]);
			Scale[i] = std::get<float>(ScaleElm.Attributes[Globals::IntToVector(i)]);

		}

		prevLoadedObjName = std::get<std::string>(NameElm.Attributes["Is"]);
		std::string TypeName = std::get<std::string>(TypeElm.Attributes["ID"]);


		ElementInfo MeshColorElm;

		MATH::Vec4 MeshColor;

		bool HasRenderer;
		if (HasRenderer = file.HasElement("Renderer"))
		{
			MeshColorElm = file.FindElement("MeshColorTint");

			for (int i = 0; i < 4; i++)
			{

				MeshColor[i] = std::get<float>(MeshColorElm.Attributes[Globals::IntToVector(i)]);

			}
		}


		for (auto obj : SaveManager::SaveableObjects)
		{
			if (TypeName == obj.first)
			{
				GameObject* clone = obj.second->GetClone();
				clone->SetName(prevLoadedObjName.data());
				clone->SetPos(Position);
				clone->SetRotation(Rotation);
				clone->SetScale(Scale);
				
				if (HasRenderer)
				{
					clone->GetComponent<MeshRenderer>()->SetColorTint(MeshColor);
				}
				
				Globals::s_SceneGraph->AddGameObject(clone);

				break;
			}
		}
	}
}

std::string LoadUtility::LoadString(std::string saveName, std::string elmName, std::string atribName)
{
	const char* QueryiedValue;
	SaveManager::GetSaveFile(saveName).FindElement(elmName).element->QueryStringAttribute(atribName.c_str(), &QueryiedValue);

	return QueryiedValue;
}

