#include "LoadUtility.h"
#include "core/Globals.h"
#include "ReflectionIncludes.h"

#include "core/CoreEngine.h"
#include "core/scene/Scene.h"
#include "core/scene/DefaultScene.h"
#include "core/GameInterface.h"

#include <filesystem>


std::unique_ptr<LoadUtility> LoadUtility::utilityInstance = std::unique_ptr<LoadUtility>();

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

LoadUtility* LoadUtility::GetInstance()
{
	if (utilityInstance == nullptr)
	{
		utilityInstance.reset(new LoadUtility);
	}
	return utilityInstance.get();
}

#pragma region Core Functions

void LoadUtility::LoadObject(SaveFile& file)
{
	if (file.GetFileType() == FileType::OBJECT)
	{
		ElementInfo PosElm = file.FindElement("Position");
		ElementInfo RotElm = file.FindElement("Rotation");
		ElementInfo ScaleElm = file.FindElement("Scale");
		ElementInfo TypeElm = file.FindElement("Type");
		ElementInfo IdentifiersElm = file.FindElement("Identifiers");
		
		MATH::Vec3 Position;
		MATH::Vec3 Rotation;
		MATH::Vec3 Scale;
		UUniqueID uuid = UUniqueID(false);

		for (int i = 0; i < 3; i++)
		{
			std::string KeyString = Globals::Engine::IntToVector(i);

			if(std::holds_alternative<float>(PosElm.Attributes[KeyString]))
			{
				Position[i] = std::get<float>(PosElm.Attributes[KeyString]);
			}
			if (std::holds_alternative<float>(RotElm.Attributes[KeyString]))
			{
				Rotation[i] = std::get<float>(RotElm.Attributes[KeyString]);
			}
			if (std::holds_alternative<float>(ScaleElm.Attributes[KeyString]))
			{
				Scale[i] = std::get<float>(ScaleElm.Attributes[KeyString]);
			}
		}

		S_PrevLoadedObjName = std::get<std::string>(IdentifiersElm.Attributes["Name"]);
		

		if (!InitalizeAttribute<uint32_t>("UniqueIdentifier", IdentifiersElm))
		{
			uuid = std::get<uint32_t>(IdentifiersElm.Attributes["UniqueIdentifier"]);
		}

		std::string TypeName = std::get<std::string>(TypeElm.Attributes["ID"]);

		ElementInfo MeshColorElm;

		MATH::Vec4 MeshColor;

		bool HasRenderer;
		if (HasRenderer = file.HasElement("classMeshRenderer"))
		{
			MeshColorElm = file.FindElement("MeshColorTint");

			for (int i = 0; i < 4; i++)
			{
				MeshColor[i] = std::get<float>(MeshColorElm.Attributes[Globals::Engine::IntToVector(i)]);
			}
		}

		for (auto obj : SaveManager::LoadableObjects)
		{
			if (TypeName == obj.first)
			{
				std::shared_ptr<GameObject> clone = obj.second->NewClone();
				clone->SetName(S_PrevLoadedObjName);
				clone->SetUUID(static_cast<uint32_t>(uuid));
				clone->SetPos(Position);
				clone->SetRotation(Rotation);
				clone->SetScale(Scale);

				if (HasRenderer)
				{
					clone->GetComponent<MeshRenderer>()->SetColorTint(MeshColor);
				}

				Globals::Engine::s_SceneGraph->AddGameObject(clone);
				break;
			}
		}
	}
}

void LoadUtility::LoadExistingSaves()
{
	std::string directory = Globals::Engine::SAVE_DATA_PATH;
	std::string objDir = "SaveData\\Objects\\";
	std::string sceneObjPath = (directory + objDir);


	EngineLogger::Info("===========EXISTING SAVES BEING LOADED===========", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);

	for (auto entry = std::filesystem::recursive_directory_iterator(directory); entry != std::filesystem::recursive_directory_iterator(); ++entry)
	{
		std::filesystem::path curPath = entry->path();

		// Makes sure were not loading in any scene files yet
		if (curPath == (sceneObjPath + curPath.filename().string()) && entry->is_directory())
		{
			entry.disable_recursion_pending();
			continue;
		}


		FileType Type = GetFileExtention(curPath.extension().string());

		if (entry->is_regular_file() && Type != FileType::DEFAULT)
		{
			LoadSave(curPath.stem().string(), curPath.string(), Type);
		}
	}

	AddLoadableObjects();

	EngineLogger::Info("===========EXISTING SAVES SUCCESFULLY LOADED===========", "SaveUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
}

void LoadUtility::LoadSceneSaves()
{
	EngineLogger::Info("===========CURRENT SCENE SAVES BEING LOADED===========", "LoadUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);

	std::string objDir = "SaveData\\Objects\\";
	std::string sceneName = CoreEngine::GetInstance()->GetCurrentScene()->GetSceneName();
	std::string sceneObjPath = (Globals::Engine::SAVE_DATA_PATH + objDir) + sceneName + "\\";

	if (!std::filesystem::exists(sceneObjPath))
		return;

	for (auto entry = std::filesystem::directory_iterator(sceneObjPath); entry != std::filesystem::directory_iterator(); ++entry)
	{
		if (entry->is_regular_file())
		{
			std::filesystem::path curPath = entry->path();

			LoadSave(curPath.stem().string(), curPath.string(), GetFileExtention(curPath.extension().string()));
		}
	}

	EngineLogger::Info("===========CURRENT SCENE SAVES SUCCESFULLY LOADED===========", "LoadUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
}

void LoadUtility::UnLoadSceneSaves()
{
	EngineLogger::Info("===========OLD SCENE SAVES BEING UNLOADED===========", "LoadUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
	for (auto elm : SaveManager::GetSaveFile(Globals::Engine::SCENE_NAME).GetElements())
	{
		SaveManager::RemoveSave(elm.first);
	}

	EngineLogger::Info("===========OLD SCENE SAVES SUCCESFULLY UNLOADED===========", "LoadUtility.cpp", __LINE__, MessageTag::TYPE_SAVE);
}

#pragma endregion

#pragma region Helper Functions

void LoadUtility::LoadRecursiveElements(tinyxml2::XMLElement* element, SaveFile& file)
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
			LoadRecursiveElements(tmpElement->FirstChildElement(), file);
		}
	}
}

void LoadUtility::AddLoadableObjects() const
{

	 SaveManager::LoadableObjects.emplace("class AiGameObject", std::make_shared<AiGameObject> ("None", MATH::Vec3())); 

	 SaveManager::LoadableObjects.emplace("class Bird", std::make_shared<Bird> ("None", MATH::Vec3())); 

	 SaveManager::LoadableObjects.emplace("class Grass", std::make_shared<Grass> ("None", MATH::Vec3())); 

	 SaveManager::LoadableObjects.emplace("class LightObject", std::make_shared<LightObject> ("None", MATH::Vec3())); 

	 SaveManager::LoadableObjects.emplace("class Player", std::make_shared<Player> ("None", MATH::Vec3())); 

	 SaveManager::LoadableObjects.emplace("class PlayerController", std::make_shared<PlayerController> ("None", MATH::Vec3())); 

	 SaveManager::LoadableObjects.emplace("class TestModel", std::make_shared<TestModel> ("None", MATH::Vec3())); 

	 SaveManager::LoadableObjects.emplace("class Box", std::make_shared<Box> ("None", MATH::Vec3())); 

	 SaveManager::LoadableObjects.emplace("class PlaneObject", std::make_shared<PlaneObject> ("None", MATH::Vec3())); 

	 SaveManager::LoadableObjects.emplace("class Sphere", std::make_shared<Sphere> ("None", MATH::Vec3())); 

	 SaveManager::LoadableObjects.emplace("class Tetrahedron", std::make_shared<Tetrahedron> ("None", MATH::Vec3())); 

} 

void LoadUtility::LoadSave(const std::string& saveName, const std::string& savePath, FileType extention)
{
	SaveFile file = SaveFile(saveName, extention);

	tinyxml2::XMLDoc SaveData;

	tinyxml2::XMLError eResult = SaveData.LoadFile(savePath.c_str());


	if (eResult != tinyxml2::XML_SUCCESS)
	{
		EngineLogger::Error("Couldn't load the Save File " + saveName, "LoadUtility.cpp", __LINE__);
	}

	tinyxml2::XMLNode* pRoot = SaveData.FirstChildElement("Root");

	LoadRecursiveElements(pRoot->FirstChildElement(), file);


	if (extention == FileType::SCENE)
	{
		file.SetHasBeenEdited(true);
	}

	SaveManager::AddToSaveFiles(saveName, SaveFile(file));
}

void LoadUtility::QueryAtributeValue(ElementInfo& info, const tinyxml2::XMLAttribute* atrib)
{
	std::string AtribName = atrib->Name();

	size_t LastOfTag = AtribName.find("_");
	
	//Get The Tag
	std::string AtribType = AtribName.substr(0, LastOfTag);
	
	//Remove Tag From Name
	AtribName = AtribName.substr(LastOfTag + 1, AtribName.length() - LastOfTag);
	if (AtribType == "F")
	{
		info.Attributes.emplace(AtribName, atrib->FloatValue());
	}
	else if (AtribType == "S")
	{
		info.Attributes.emplace(AtribName, std::string(atrib->Value()));
	}
	else if (AtribType == "U64")
	{
		uint32_t cast = static_cast<uint32_t>(atrib->Unsigned64Value());

		info.Attributes.emplace(AtribName, cast);
	}
	else if (AtribType == "I")
	{
		info.Attributes.emplace(AtribName, atrib->IntValue());
	}
	else if (AtribType == "B")
	{
		info.Attributes.emplace(AtribName, atrib->BoolValue());
	}

	else
	{
		EngineLogger::Info("Attribute type unable to be queried", "LoadUtility.cpp", __LINE__);
	}
}

FileType LoadUtility::GetFileExtention(const std::string& ext) const
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


#pragma endregion


void LoadUtility::LoadDefaultScenes(Game* G_Interface) const
{
	std::vector<SaveFile> sceneFiles = SaveManager::GetSavesOfType(FileType::SCENE);
	
	for (size_t i = 0; i < sceneFiles.size(); i++)
	{
		for (size_t s = 0; s < G_Interface->Scenes.size(); s++)
		{
			std::string classType = std::get<std::string>(sceneFiles[i].FindAttribute("BaseClass:", ":"));
			if (typeid(*G_Interface->Scenes[s]).name() == classType)
			{
				G_Interface->Scenes[s]->SetSceneName(sceneFiles[i].GetFileName());
				
			}
		}
		
	}

	// Adds all scenes that use the DefaultScene Class
	for (auto save : sceneFiles)
	{
		std::string classType = std::get<std::string>(save.FindAttribute("BaseClass:", ":"));
		if (classType == "class DefaultScene")
		{
			G_Interface->Scenes.push_back(new DefaultScene(save.GetFileName()));
		}
	}
}

void LoadUtility::RemoveAttributePrefixs(const std::string& saveName)
{


	for (auto& element : SaveManager::GetSaveFile(saveName).GetElements())
	{
		std::map<std::string, Attribute> AtribMap;
		for (auto const& attribute : element.second.Attributes)
		{


			auto node = element.second.Attributes.find(attribute.first);


			
			std::string AtribName = attribute.first;

			size_t LastOfTag = AtribName.find("_") + 1;


			//Get The Tag
			AtribName = AtribName.substr(LastOfTag , AtribName.size() - LastOfTag);


			AtribMap.emplace(AtribName, attribute.second);
			

			

		}
		if (AtribMap.size() > 0)
		{
			element.second.Attributes = AtribMap;

		}

	}
}

