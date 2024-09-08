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
	FString FClassUtils::GetClassDescription(const UClass* Class)
	{
		check(Class);
		if (Class->HasAllClassFlags(CLASS_Interface))
		{
			return "## UInterface cannot be documented ##";
		}

		FString ClassToolTip = Class->GetToolTipText().ToString();
		if (ClassToolTip != FBlueprintEditorUtils::GetFriendlyClassDisplayName(Class).ToString())
		{
			return ClassToolTip;
		}
		return "";
	}

	FString FClassUtils::GetClassDocName(const UClass* Class)
	{
		check(Class);
		FString Name = Class->GetName();
		if (!Class->HasAnyClassFlags(CLASS_Native))
		{
			Name.RemoveFromEnd(TEXT("_C"));
			Name.RemoveFromStart(TEXT("SKEL_"));
		}
		static const FName DisplayName("DisplayName");
		if (Class->HasMetaData(DisplayName))
		{
			return Name + " (" + Class->GetDisplayNameText().ToString() + ")";
		}
		return Name;
	}

	FString FClassUtils::GetNodeDescription(UEdGraphNode* Node)
	{
		check(Node);
		FString NodeDesc = Node->GetTooltipText().ToString();
		if (NodeDesc != Node->GetClass()->GetToolTipText().ToString())
		{
			int32 TargetIdx = NodeDesc.Find(TEXT("Target is "), ESearchCase::CaseSensitive);
			if (TargetIdx != INDEX_NONE)
			{
				NodeDesc = NodeDesc.Left(TargetIdx).TrimEnd();
			}
			return NodeDesc;
		}
		return "";
	}
};
