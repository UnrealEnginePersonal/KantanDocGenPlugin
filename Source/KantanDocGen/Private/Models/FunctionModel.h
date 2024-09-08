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
	struct FFunctionModel final : FBaseModel
	{
	public:
		FString FullName;
		FString DisplayName;
		FString Image;
		bool bBlueprintCallable;
		bool bBlueprintPure;
		bool bBlueprintEvent;

		TArray<TSharedPtr<FPropertyModel>> Parameters;

		// TODO: Add These Properties
		FString RawDescription;
		FString RawShortDescription;

		FFunctionModel(const FName& InName, const FString& Description);
		FFunctionModel(const FName& InName, const FString& InFullName, const FString& InDescription,
					   const FString& InImage, const bool bInBlueprintCallable, const bool bInBlueprintPure,
					   const bool bInBlueprintEvent);

		virtual ~FFunctionModel() override = default;

		virtual FJsonObject ToJson() const override;

		void AddParameter(const FPropertyModel& Parameter);
	};
} // namespace Kds::DocGen::Models
