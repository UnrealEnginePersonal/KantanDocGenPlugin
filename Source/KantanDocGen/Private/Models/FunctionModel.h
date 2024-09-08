// /***********************************************************************************
// *  File:             FunctionModel.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          08-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once
#include "BaseModel.h"
#include "PropertyModel.h"

namespace Kds::DocGen::Models
{
	struct FFunctionModel : FBaseModel
	{
	public:
		FString FullName;
		FString DisplayName;
		FString Image;
		bool bBlueprintCallable;
		bool bBlueprintEvent;
		TArray<FPropertyModel> Parameters;

		FFunctionModel(const FName& InName, const FString& InFullName, const FString& InDescription,
					   const FString& InImage, const bool bInBlueprintCallable, const bool bInBlueprintEvent);

		virtual ~FFunctionModel() override;

		void AddParameter(const FPropertyModel& Parameter);
	};
} // namespace Kds::DocGen::Models
