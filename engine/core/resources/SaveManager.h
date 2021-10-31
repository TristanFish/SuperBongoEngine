#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "SaveFile.h"
#include <unordered_map>

class GameObject;
class SaveManager
{

private:

	static std::unordered_map<std::string, SaveFile> SaveFiles;
	static std::unordered_map<std::string, SaveFile> SaveQueue;

	static void AddToSaveFiles(const std::string& name, const  SaveFile& File);
	static void RemoveSave(const std::string& saveName);


public:

	static std::unordered_map<std::string, GameObject*> SaveableObjects;
	static std::vector<SaveFile> GetSavesOfType(FileType type);

	static SaveFile& GetSaveFile(const std::string saveName);
	static void SetSaveName(const std::string old_Name, const std::string new_Name);

	static bool HasSave(const std::string& saveName);
	static bool TransferToSaveQueue(const std::string& saveName);

	static void SaveAll();
	static void AddToSaveQueue(const std::string& name, const  SaveFile& File);

	static void DeleteSaveableObjects();

	friend class SaveUtility;
	friend class LoadUtility;
};

#endif
