#ifndef LOADUTILITY_H
#define LOADUTILITY_H

#include <memory>

#include "core//resources/SaveManager.h"


class LoadUtility
{
private:

	static std::unique_ptr<LoadUtility> utilityInstance;
	friend std::default_delete<LoadUtility>;


	void LoadExistingSave(std::string fileName);
public:

	LoadUtility();
	~LoadUtility();

	static LoadUtility* GetInstance();


	
};

#endif

