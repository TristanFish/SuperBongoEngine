#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <iostream>
#include "core/Logger.h"
#include <variant>
#include <map>
#include <vector>
#include "tinyxml2/tinyxml2.h" 





typedef std::variant<int, float, const char*,double> Attribute;


//! FileType enum class
/*! Used to differentiate file types so that we can save unique information to specific file types */
enum class FileType 
{
	DEFAULT,
	SCENE,
	OBJECT,
};


//! ElementInfo Class
/*! Holds all the needed information for an element */
struct ElementInfo
{
	//! XMLElement pointer
	/*! Holds a pointer to and XMLElement that allows us to use tinyxml2 */
	tinyxml2::XMLElement* element;

	//!ParentName string
	/*!This is used so that we know where an element needs to be saved in the XML Doc*/
	std::string parentName;


	//! Attributes map
	/*! Hold's all of the attributes that an element can have. For instance the element could be "Position" and the attributes could be X,Y,Z */
	std::map<std::string, Attribute> Attributes;

	//! Standard Constructor
	/*!Initializes the structs variables. */
	inline ElementInfo() : parentName(""), element(nullptr)
	{
	
	}

	//! Alternate Constructor
	/*!Initializes the structs variables. and sets the parent name to the passed in string*/
	inline ElementInfo(std::string parentName_) : element(nullptr)
	{
		parentName = parentName_;
	}

	//! Standard Destructor
	/*!Destroys any of the needed pointers and clears the attributes*/
	inline ~ElementInfo()
	{
		element = nullptr;

		if (Attributes.size() > 0)
		{
			Attributes.clear();
		}
	}


	//! HasParent Function
	/*!Returns if the parent name is empty or not.*/
	inline bool HasParent() { return parentName.empty(); }


	//! IsRootChild Function
	/*!Returns if the parent name is equal to "Root"*/
	inline bool IsRootChild() { return parentName == "Root"; }
};


//! SaveFile Class
/*!Hold's all of the needed variables and functions to allow us to save all this classes data to an xml file*/
class SaveFile {

private:



	//! string FileName 
	/*!Holds's the name of the file. This is mostly used for saving*/
	std::string FileName;


	//!XMLDoc Doc
	/*!Is a tinyxml2 class that is used to save all the information too and then save it as an xml document*/
	tinyxml2::XMLDoc Doc;

	//!XMLNode RootNode
	/*!Hold's a pointer to the first node in every xml document*/
	tinyxml2::XMLNode* rootNode;

	//! vector Insertion Order
	/*!Hold's the order in which elements are inserted. This is used in the compile saves function in "SaveUtility" so we can loop though all the elements correctly*/
	std::vector<std::string> insertionOrder;

	//! map Elements
	/*!Hold's all of the elements that are contained in the xml file*/
	std::map<std::string,  ElementInfo> Elements;
	
	//! FileType
	/*!Stores what type of file that this is.*/
	FileType fileType;


private:

	//! InitalizeFile Function
	/*!Initializes the classes variables and calls any needed functions (Called In constructor)*/
	void InitalizeFile();

	//! GetFileType Function
	/*!Returns what file type this SaveFile will be saved as when converting to xml*/
	std::string GetFileType();

	//! GetFileDestination Function
	/*!Returns where the file will be saved to*/
	std::string GetFileDestination();

public:

	//! SaveFile Constructor
	/*!Standard constructor that initializes any needed variables*/
	SaveFile();

	//! SaveFile Alternate Constructor
	/*!Initializes the save file variables and sets the filename and type*/
	SaveFile(const std::string& FileName_, const FileType type = FileType::DEFAULT);

	//! SaveFile Alternate Constructor
	/*!Initializes the save file variables and sets the filename all its elements and the Doc*/
	SaveFile(std::string& FileName_, std::map<std::string, ElementInfo> Elements_, tinyxml2::XMLDoc& Doc_);

	//! SaveFile Copy Constructor
	/*!Copy's the given save file's variables to this save file*/
	SaveFile(const SaveFile& file);

	//! SaveFile Destructor
	~SaveFile();


	//! SaveFile Equals operator
	/*!Copy's the given save file's variables to this save file*/
	SaveFile operator =(const SaveFile& file);

	//! SaveFile Equals Equals operator
	/*!Returns true if the save files are equal to each other*/
	bool operator ==(const SaveFile& file);


	//! Save Function
	/*!Attempts to save all the information to a file path.*/
	void Save();

	//! FindElement Function
	/*!Returns the address to an element with a given name*/
	ElementInfo& FindElement(std::string elmName);

	//! AddElement Function
	/*!Adds an element to the save file*/
	 void AddElement(const std::string name, const ElementInfo element);

	 //! AddElements Function
	/*!Adds elements to the save file*/
	 void AddElements(const std::map<std::string, ElementInfo> elements);

	 //! AddAttribute Function
	/*!Adds a attribute to a given element*/
	 void AddAttribute(const std::string elmName, const std::string atribName, Attribute value);

	 //! AddAttributes Function
	 /*!Adds attributes to a given element*/
	 void AddAttributes(const std::string elmName, std::map<std::string, Attribute> attributes);


	 //! GetFileType Function
	 /*!Returns this SaveFile's type*/
	 inline FileType GetFileType() const { return fileType; }

	 //! SetFileType Function
	 /*!Sets this SaveFile's type*/
	 inline void SetFileType(FileType type) { fileType = type; }



	 friend class SaveUtility;
};



#endif
