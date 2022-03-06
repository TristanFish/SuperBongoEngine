#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "SaveFile.h"
#include <unordered_map>

class GameObject;


//! SaveManager class
/*! Static class that stores all of the Save Systems data including {Loaded Save Files, Save Files Queued for save, Saveable Objects, etc} */
class SaveManager
{

private:

	//! SaveFiles unordered_map
    /*! Stores all of our currently loaded save files that haven't been changed. */
	static std::unordered_map<std::string, SaveFile> SaveFiles;

	//! SaveQueue unordered_map
	/*! Stores all of our currently loaded save files that have recently been changed. */
	static std::unordered_map<std::string, SaveFile> SaveQueue;

	//! AddToSaveFiles function
	/*! Add's a new save file to the SaveFiles unordered_map */
	static void AddToSaveFiles(const std::string& name, const  SaveFile& File);

	//! AddToSaveQueue function
	/*! Add's a new save file to the SaveQueue unordered_map*/
	static void AddToSaveQueue(const std::string& name, const  SaveFile& File);

	//! RemoveSave function
	/*! Looks through the SaveFiles and SaveQueue unordered_map and removes an item with th given key */
	static void RemoveSave(const std::string& saveName);


public:

	//! SaveableObjects unordered_map
	/*! Stores all of our gameobject types than can be saved to a file */
	static std::unordered_map<std::string, std::shared_ptr<GameObject>> SaveableObjects;

	//! GetSavesOfType function
	/*! Returns a list of SaveFiles with a specific type */
	static std::vector<SaveFile> GetSavesOfType(const FileType& type);


	//! GetSaveFile Getter
	/*! Returns the savefile with a given name */
	static SaveFile& GetSaveFile(const std::string saveName);

	//! SetSaveName function
	/*! Copies the KeyValue pair with the old name and changes the key to the new_Name before insertion again*/
	static void SetSaveName(const std::string& old_Name, const std::string& new_Name);


	//! HasSave function
	/*! Returns true if either unorderd_maps contain a save with the given name*/
	static bool HasSave(const std::string& saveName);

	//! TransferToSaveQueue function
	/*! Returns true if the save with the given name exists and the Key/Value pair was transfered*/
	static bool TransferToSaveQueue(const std::string& saveName);


	//! SaveAll function
	/*! Move's the key/value pairs from the save queue back into the SaveFiles list*/
	static void SaveAll();

	
	//! DeleteSaveableObjects function
	/*! Set's all gameobject pointers to null in the SaveableObjects unordered_map and clears it*/
	static void DeleteSaveableObjects();

	friend class SaveUtility;
	friend class LoadUtility;
};

#endif
