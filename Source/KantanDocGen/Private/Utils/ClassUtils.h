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
			static FString GetClassDescription(const UClass* Class);
			static FString GetClassDocName(const UClass* Class);
			static FString GetNodeDescription(UEdGraphNode* Node);
		};
	}
}
