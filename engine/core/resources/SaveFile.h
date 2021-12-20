#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <iostream>
#include "core/Logger.h"
#include <variant>
#include <map>
#include <vector>
#include "tinyxml2/tinyxml2.h" 





typedef std::variant<int, float,double,bool,std::string> Attribute;


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
	/*! Holds all of the attributes that an element can have. For instance the element could be "Position" and the attributes could be X,Y,Z */
	std::map<std::string, Attribute> Attributes;

	//! Standard Constructor
	/*!Initializes the structs variables. */
	ElementInfo() : element(nullptr), parentName(""), Attributes(std::map<std::string, Attribute>())
	{
	
	}

	//! Alternate Constructor
	/*!Initializes the structs variables. and sets the parent name to the passed in string*/
	ElementInfo(const std::string& parentName_) : element(nullptr), Attributes(std::map<std::string, Attribute>())
	{
		parentName = parentName_;
	}

	//! Standard Destructor
	/*!Destroys any of the needed pointers and clears the attributes*/
	~ElementInfo()
	{
		element = nullptr;

		if (!Attributes.empty())
		{
			Attributes.clear();
		}
	}

	bool operator != (const ElementInfo & elm)
	{
		for (const auto& m_atrib : Attributes)
		{
			for (const auto& s_atrib : elm.Attributes)
			{
				if (m_atrib.first == s_atrib.first)
				{
					if (m_atrib.second != s_atrib.second)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	//! HasParent Function
	/*!Returns if the parent name is empty or not.*/
	bool HasParent() const { return parentName.empty(); }


	//! IsRootChild Function
	/*!Returns if the parent name is equal to "Root"*/
	bool IsRootChild() const { return parentName == "Root"; }
};


//! SaveFile Class
/*!Holds all of the needed variables and functions to allow us to save all this classes data to an xml file*/
class SaveFile {

private:



	//! string FileName 
	/*!Holds the name of the file. This is mostly used for saving*/
	std::string FileName;

	//! string PreviousFileName 
	/*!Holds the file name when the application started*/
	std::string prevFileName;

	//!XMLDoc Doc
	/*!Is a tinyxml2 class that is used to save all the information too and then save it as an xml document*/
	tinyxml2::XMLDoc Doc;

	//!XMLNode RootNode
	/*!Holds a pointer to the first node in every xml document*/
	tinyxml2::XMLNode* rootNode;

	//! vector Insertion Order
	/*!Holds the order in which elements are inserted. This is used in the compile saves function in "SaveUtility" so we can loop though all the elements correctly*/
	std::vector<std::string> insertionOrder;

	//! map Elements
	/*!Holds all of the elements that are contained in the xml file*/
	std::map<std::string,  ElementInfo> Elements;
	
	//! FileType
	/*!Stores what type of file that this is.*/
	FileType fileType;

	//! HasBeenEdited bool
	/*!Is used when saving the save files so that we know what files need to be saved*/
	bool HasBeenEdited;

private:

	//! InitalizeFile Function
	/*!Initializes the classes variables and calls any needed functions (Called In constructor)*/
	void InitalizeFile();

	//! GetFileType Function
	/*!Returns what file type this SaveFile will be saved as when converting to xml*/
	std::string GetSaveFileType() const;

	//! GetFileDestination Function
	/*!Returns where the file will be saved to*/
	std::string GetFileDestination() const;


	
public:

	//! SaveFile Constructor
	/*!Standard constructor that initializes any needed variables*/
	SaveFile();

	//! SaveFile Alternate Constructor
	/*!Initializes the save file variables and sets the filename and type*/
	SaveFile(const std::string& FileName_, FileType type = FileType::DEFAULT);

	//! SaveFile Alternate Constructor
	/*!Initializes the save file variables and sets the filename all its elements and the Doc*/
	SaveFile(const std::string& FileName_, const std::map<std::string, ElementInfo>& Elements_, tinyxml2::XMLDoc& Doc_);

	//! SaveFile Copy Constructor
	/*!Copies the given save file's variables to this save file*/
	SaveFile(const SaveFile& file);

	//! SaveFile Destructor
	~SaveFile();


	//! SaveFile Equals operator
	/*!Copies the given save file's variables to this save file*/
	SaveFile operator =(const SaveFile& file);

	//! SaveFile Equals Equals operator
	/*!Returns true if the save files are equal to each other*/
	bool operator ==(const SaveFile& file) const;


	//! Save Function
	/*!Attempts to save all the information to a file path.*/
	void Save();

	//! FindElement Function
	/*!Returns the address to an element with a given name*/
	ElementInfo& FindElement(const std::string& elmName);

	//! HasElement Function
	/*!Returns if this savefile contains the element with a given name*/
	bool HasElement(const std::string& elmName) const;

	inline bool GetHasBeenEdited() const { return HasBeenEdited; }

	inline void SetHasBeenEdited(bool HasBeenEdited_) { HasBeenEdited = HasBeenEdited_; }

	//! FindElement Function
	/*!Returns the address to an element with a given name*/
	Attribute& FindAttribute(const std::string& elmName, const std::string& atrbName);

	//! AddElement Function
	/*!Adds an element to the save file*/
	 void AddElement(const std::string& name, ElementInfo element);

	 //! AddElements Function
	/*!Adds elements to the save file*/
	 void AddElements(const std::map<std::string, ElementInfo>& elements);

	 //! AddAttribute Function
	/*!Adds a attribute to a given element*/
	 void AddAttribute(const std::string& elmName, const std::string& atribName, Attribute value);

	 //! AddAttributes Function
	 /*!Adds attributes to a given element*/
	 void AddAttributes(const std::string& elmName, const std::map<std::string, Attribute>& attributes);

	 void SetElementName(const std::string& o_ElmName, const std::string& n_ElmName);

	 void RemoveElement(const std::string& name);

	 void ClearElements();

	 void ClearSaveFile();

	 inline std::string GetFileName() const { return FileName; }

	 inline void SetFileName(const std::string& newName) { FileName = newName; }

	 //! GetFileType Function
	 /*!Returns this SaveFile's type*/
	 inline FileType GetFileType() const { return fileType; }

	 //! SetFileType Function
	 /*!Sets this SaveFile's type*/
	 inline void SetFileType(FileType type) { fileType = type; }

	 inline std::map<std::string, ElementInfo>& GetElements() { return Elements; }

	 friend class SaveUtility;
};



#endif
