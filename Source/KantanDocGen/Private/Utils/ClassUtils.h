// /***********************************************************************************
// *  File:             ClassUtils.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

namespace Kds::DocGen
{
	namespace Utils
	{
		class FClassUtils
		{
		public:
			static FName GetClassName(const UClass* Class);
			static FString GetClassNameString(const UClass* Class);
			static FString GetClassDisplayName(const UClass* Class);
			static FString GetClassTree(const UClass* Class);
			static FString GetClassIncludePath(const UClass* Class);
			static FString GetClassDescription(const UClass* Class);
			static FString GetClassDisplayNameElseName(const UClass* Class);
			static FString GetClassDocName(const UClass* Class);
			static FString GetNodeDescription(const UEdGraphNode* Node);
			
		};
	}
}
