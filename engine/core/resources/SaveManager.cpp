#include "SaveManager.h"
#include "components/GameObject.h"
std::unordered_map<std::string, SaveFile> SaveManager::SaveFiles = std::unordered_map<std::string, SaveFile>();

std::unordered_map<std::string, SaveFile> SaveManager::SaveQueue = std::unordered_map<std::string, SaveFile>();
std::unordered_map<std::string, GameObject*> SaveManager::SaveableObjects = std::unordered_map<std::string, GameObject*>();





void SaveManager::AddToSaveFiles(const std::string& name, const SaveFile& File)
{
	EngineLogger::Save(name + " Has Been Added To The SaveFiles", "SaveManager.cpp", __LINE__);
	SaveFiles.emplace(name, File);
}

void SaveManager::RemoveSave(const std::string saveName)
{
	if (SaveFiles.size() > 1)
	{
		if (SaveFiles.erase(saveName))
		{
			EngineLogger::Save(saveName + " File Has Been Deleted", "SaveManager.cpp", __LINE__);
		}
		else {
			EngineLogger::Save(saveName + " File Has Not Been Located", "SaveManager.cpp", __LINE__);

		}
	}
}



SaveFile& SaveManager::GetSaveFile(const std::string saveName)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveFiles.find(saveName);

	while (iter != SaveFiles.end())
	{

		if (iter->first == saveName)
		{
			return iter->second;
		}
		iter++;
	}
}

bool SaveManager::HasSave(const std::string& saveName)
{
	std::unordered_map<std::string, SaveFile>::iterator saveFilesIter = SaveFiles.begin();


	while (saveFilesIter != SaveFiles.end())
	{

		if (saveFilesIter->first == saveName)
		{
			return true;
		}

		saveFilesIter++;
	}

	return false;
}

void SaveManager::SaveAll()
{
	std::unordered_map<std::string, SaveFile>::iterator saveQueueIter = SaveQueue.begin();
	std::unordered_map<std::string, SaveFile>::iterator saveFilesIter = SaveFiles.begin();


	// Copies over the save file's from the queue to the SaveFiles  
	while (saveQueueIter != SaveQueue.end())
	{
		saveFilesIter = SaveFiles.find(saveQueueIter->first);

		if (saveFilesIter != SaveFiles.end())
		{
			saveFilesIter->second = saveQueueIter->second;
		}
		else
		{
			SaveFiles.emplace(saveQueueIter->first,saveQueueIter->second);
		}
		saveQueueIter++;
	}

	

	SaveQueue.clear();
}

void SaveManager::AddToSaveQueue( const std::string&name, const SaveFile& File)
{
	SaveQueue.emplace(name, File);
}

void SaveManager::TransferToSaveQueue(const std::string& saveName)
{
	std::unordered_map<std::string, SaveFile>::iterator saveFilesIter = SaveFiles.begin();


	while (saveFilesIter != SaveFiles.end())
	{

		if (saveFilesIter->first == saveName)
		{
			SaveQueue.emplace(saveFilesIter->first, saveFilesIter->second);

			RemoveSave(saveName);
			break;
		}

		saveFilesIter++;
	}		

}
