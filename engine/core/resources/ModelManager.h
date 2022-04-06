#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <unordered_map>
#include "graphics/Model.h"

//! Model Manager Class
/*!  Manages all of the models in a given scene */
class ModelManager
{
public:

	ModelManager();
	~ModelManager();

	//!Static GetModel Getter
	/*!  Returns a model with a given name/filepath */
	 Model&  GetModel(const std::string& name);

	//! Static DestroyAllModels Function
	/*!  Destroys all of the models in the "models" unordered_map */

	static ModelManager* GetInstance();


private:

	static std::unique_ptr<ModelManager> modelManagerInstance;
	friend std::default_delete<ModelManager>;


	 void DestroyAllModels();

	//! Static Models unordered_map
	/*!  Stores all of the models in our scene */
	 std::unordered_map<std::string, Model> models;

	//! Static LoadAllModels Function
	/*! Loads all the models when given the filepath */
	 void LoadAllModels();

	//! Friend CoreEngine
	/*! Enables us to access all of the variables in the "CoreEngine" */
	friend class CoreEngine;
};
#endif



