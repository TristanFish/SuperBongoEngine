#include "SaveManager.h"
#include "components/GameObject.h"
std::unordered_map<std::string, SaveFile> SaveManager::SaveFiles = std::unordered_map<std::string, SaveFile>();

std::unordered_map<std::string, SaveFile> SaveManager::SaveQueue = std::unordered_map<std::string, SaveFile>();
std::unordered_map<std::string, GameObject*> SaveManager::SaveableObjects = std::unordered_map<std::string, GameObject*>();





std::vector<SaveFile> SaveManager::GetSavesOfType(FileType type)
{
	std::vector<SaveFile> filesOfType;

	std::unordered_map<std::string, SaveFile>::iterator iter = SaveFiles.begin();
	std::unordered_map<std::string, SaveFile>::iterator iterQueue = SaveQueue.begin();

	while (iter != SaveFiles.end())
	{
		if (iter->second.GetFileType() == type)
		{
			filesOfType.push_back(iter->second);
		}
		iter++;
	}

	while (iterQueue != SaveQueue.end())
	{
		if (iter->second.GetFileType() == type)
		{
			filesOfType.push_back(iter->second);
		}
		iter++;
	}

	return filesOfType;
}

void SaveManager::AddToSaveFiles(const std::string& name, const SaveFile& File)
{
	EngineLogger::Save(name + " Has Been Added To The SaveFiles", "SaveManager.cpp", __LINE__);
	SaveFiles.emplace(name, File);
}

void SaveManager::RemoveSave(const std::string saveName)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveFiles.find(saveName);
	std::unordered_map<std::string, SaveFile>::iterator iterQueue = SaveQueue.find(saveName);

	if (SaveFiles.size() < 1)
	{
		return;
	}


	if (iter != SaveFiles.end())
	{
		SaveFiles.erase(iter);
		EngineLogger::Save(saveName + " File Has Been Deleted", "SaveManager.cpp", __LINE__);
	}

	else if (iterQueue != SaveQueue.end())
	{
		SaveQueue.erase(iterQueue);
		EngineLogger::Save(saveName + " File Has Been Deleted", "SaveManager.cpp", __LINE__);		
	}
}



SaveFile& SaveManager::GetSaveFile(const std::string saveName)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveFiles.find(saveName);
	std::unordered_map<std::string, SaveFile>::iterator iterQueue = SaveQueue.find(saveName);

	if(iter != SaveFiles.end())
	{
		return iter->second;
	}
	else if (iterQueue != SaveQueue.end())
	{
		return iterQueue->second;
	}
}

void SaveManager::SetSaveName(const std::string old_Name, const std::string new_Name)
{
	std::unordered_map<std::string, SaveFile>::iterator iter = SaveFiles.find(old_Name);
	std::unordered_map<std::string, SaveFile>::iterator queueIter = SaveQueue.find(old_Name);




	if (iter != SaveFiles.end())
	{
		iter->second.SetFileName(new_Name);
		auto node = SaveFiles.extract(old_Name);
		if (!node.empty())
		{
			node.key() = new_Name;
			
			SaveFiles.insert(std::move(node));
		}
	}

	else if (queueIter != SaveQueue.end())
	{
		queueIter->second.SetFileName(new_Name);

		auto node = SaveQueue.extract(old_Name);
		if (!node.empty())
		{
			node.key() = new_Name;
			SaveQueue.insert(std::move(node));
		}
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

bool SaveManager::TransferToSaveQueue(const std::string& saveName)
{
	std::unordered_map<std::string, SaveFile>::iterator saveFilesIter = SaveFiles.begin();


	while (saveFilesIter != SaveFiles.end())
	{

		if (saveFilesIter->first == saveName)
		{
			SaveQueue.emplace(saveFilesIter->first, saveFilesIter->second);

			RemoveSave(saveName);
			return true;
		}

		saveFilesIter++;
	}

	return false;

}
