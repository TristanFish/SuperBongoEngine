#include "Component.h"
#include <Utility/SaveUtility.h>

void Component::OnSaveComponent(const std::string& saveName, std::string parentName)
{
	std::string typeName = typeid(*this).name();
	typeName.erase(std::remove_if(typeName.begin(), typeName.end(), std::isspace), typeName.end());
	typeName.erase(std::remove(typeName.begin(), typeName.end(), '*'), typeName.end());

	SaveUtility::GetInstance()->AddElement(saveName, typeName, ElementInfo(parentName));

	ElementInfo component = ElementInfo(typeName);

	component.Attributes.emplace("B_Active", active);
	SaveUtility::GetInstance()->AddElement(saveName, "IsActive", component);
}
