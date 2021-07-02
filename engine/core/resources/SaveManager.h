#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "SaveFile.h"
#include <unordered_map>
class SaveManager
{

private:

	static std::unordered_map<std::string, SaveFile> SaveFiles;
	static std::unordered_map<std::string, SaveFile> SaveQueue;

	static void RemoveSave(const std::string saveName);


public:

	static SaveFile& GetSaveFile(const std::string saveName);

	static void SaveAll();
	static void AddToSaveQueue(const std::string& name, const  SaveFile& File);


	friend class SaveUtility;
};

#endif
