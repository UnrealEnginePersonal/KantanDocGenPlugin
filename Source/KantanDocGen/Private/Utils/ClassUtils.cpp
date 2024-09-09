// /***********************************************************************************
// *  File:             ClassUtils.cpp
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#include "ClassUtils.h"

#include "Kismet2/BlueprintEditorUtils.h"

namespace Kds::DocGen::Utils
{
	
	FName FClassUtils::GetClassName(const UClass* Class)
	{
		return Class->GetFName();
	}
	
	FString FClassUtils::GetClassNameString(const UClass* Class)
	{
		return GetClassName(Class).ToString();
	}
	
	FString FClassUtils::GetClassDescription(const UClass* Class)
	{
		return Class->GetToolTipText(false).ToString();
	}
	

	FString FClassUtils::GetClassDisplayName(const UClass* Class)
	{
		
		static const FName DisplayName("DisplayName");
		if (Class->HasMetaData(DisplayName))
		{
			return Class->GetMetaData(DisplayName);
		}

		return FBlueprintEditorUtils::GetFriendlyClassDisplayName(Class).ToString();
	}

	
	FString FClassUtils::GetClassTree(const UClass* Class)
	{
		FString ClassTreeStr = *FClassUtils::GetClassDocName(Class);
		const UClass* Parent = Class->GetSuperClass();

		while (nullptr != Parent)
		{
			ClassTreeStr = FString::Printf(TEXT("%s > %s"), *FClassUtils::GetClassDocName(Parent), *ClassTreeStr);
			Parent = Parent->GetSuperClass();
		}

		return ClassTreeStr;
	}

	FString FClassUtils::GetClassIncludePath(const UClass* Class)
	{
		static const FName PathKey("ModuleRelativePath");
		FString Path = Class->GetMetaData(PathKey);

		if (Path.IsEmpty())
		{
			Path = Class->GetPathName();
			Path.RemoveFromEnd("." + Class->GetName());
		}

		return Path;
	}

	

	FString FClassUtils::GetClassDisplayNameElseName(const UClass* Class)
	{
		if (const FString DisplayName = GetClassDisplayName(Class); !DisplayName.IsEmpty())
		{
			return DisplayName;
		}
		return GetClassNameString(Class);
	}

	FString FClassUtils::GetClassDocName(const UClass* Class)
	{
		FString Name = GetClassNameString(Class);
		if (!Class->HasAnyClassFlags(CLASS_Native))
		{
			Name.RemoveFromEnd(TEXT("_C"));
			Name.RemoveFromStart(TEXT("SKEL_"));
		}

		if (const FString DisplayName = GetClassDisplayName(Class); !DisplayName.IsEmpty())
		{
			return Name + " (" + DisplayName + ")";
		}
		return Name;
	}

	FString FClassUtils::GetNodeDescription(const UEdGraphNode* Node)
	{
		if (FString NodeDesc = Node->GetTooltipText().ToString();
			NodeDesc != Node->GetClass()->GetToolTipText().ToString())
		{
			if (const int32 TargetIdx = NodeDesc.Find(TEXT("Target is "), ESearchCase::CaseSensitive);
				TargetIdx != INDEX_NONE)
			{
				NodeDesc = NodeDesc.Left(TargetIdx).TrimEnd();
			}
			return NodeDesc;
		}
		return "No description available.";
	}
}; // namespace Kds::DocGen::Utils
