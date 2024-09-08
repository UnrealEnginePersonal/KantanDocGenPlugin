// /***********************************************************************************
// *  File:             EventModel.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          08-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "PropertyModel.h"
#include "BaseModel.h"

namespace Kds::DocGen::Models
{
	struct FEventModel : FBaseModel
	{
	public:
		const FString FullName;
		const FString Image;
		const bool bBlueprintCallable;
		const bool bBlueprintEvent;
		TArray<FPropertyModel> Parameters;

		FEventModel(const FName InName, const FString& InFullName, const FString& InDescription, const FString& InImage,
					const bool bInBlueprintCallable, const bool bInBlueprintEvent);

		~FEventModel() = default;

		void AddParameter(const FPropertyModel&& Parameter);
	};
} // namespace Kds::DocGen::Models
