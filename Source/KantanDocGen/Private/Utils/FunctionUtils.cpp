#include "FunctionUtils.h"

#include "Kismet2/BlueprintEditorUtils.h"
#include "Models/FunctionModel.h"


FName FFunctionUtils::GetFunctionName(const UFunction* Function)
{
	return Function->GetFName();
}

FString FFunctionUtils::GetFunctionFullName(const UFunction* Function)
{
	return Function->GetFullName();
}

FString FFunctionUtils::GetFunctionNameString(const UFunction* Function)
{
	return GetFunctionName(Function).ToString();
}
FString FFunctionUtils::GetFunctionDisplayName(const UFunction* Function)
{
	static const FName DisplayName("DisplayName");
	if (Function->HasMetaData(DisplayName))
	{
		return Function->GetMetaData(DisplayName);
	}

	return Function->GetDisplayNameText().ToString();
}
FString FFunctionUtils::GetFunctionRawDescription(const UFunction* Function) 
{
	return Function->GetToolTipText(false).ToString();
}
FString FFunctionUtils::GetFunctionRawShortDescription(const UFunction* Function)
{
	return Function->GetToolTipText(true).ToString();
}
FString FFunctionUtils::GetFunctionDescription(const UFunction* Function)
{
	return Function->GetToolTipText(false).ToString();
}
