#include "Component.h"
#include <Utility/SaveUtility.h>

void Component::OnSaveComponent(const std::string& saveName, std::string parentName)
{
	std::string ClassName = GetComponentName();

	SaveUtility::GetInstance()->AddElement(saveName, ClassName, ElementInfo(parentName));

	SaveManager::GetSaveFile(saveName).FindElement(ClassName).Attributes.emplace("B_Active", active);
}

std::string Component::GetComponentName()
{
	std::string typeName = typeid(*this).name();
	typeName.erase(std::remove_if(typeName.begin(), typeName.end(), std::isspace), typeName.end());
	typeName.erase(std::remove(typeName.begin(), typeName.end(), '*'), typeName.end());

	return typeName;
}
