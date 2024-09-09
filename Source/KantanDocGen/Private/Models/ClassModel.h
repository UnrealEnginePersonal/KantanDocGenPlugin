// /***********************************************************************************
// *  File:             ClassModel.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          08-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "BaseModel.h"

namespace Kds::DocGen::Models
{
	struct FPropertyModel;
	struct FEventModel;
	struct FFunctionModel;

	struct FClassModel final : FBaseModel
	{
		FClassModel(const FName& InName, const FString& InDescription);
		virtual ~FClassModel() override = default;

		void AddProperty(const FPropertyModel& Property);
		void AddEvent(const FFunctionModel& Event);
		void AddFunction(const FFunctionModel& Function);
		
		FString DisplayName;
		FString IncludePath;
		FString ClassTree;

		TArray<TSharedPtr<FPropertyModel>> Properties;
		TArray<TSharedPtr<FFunctionModel>> Events;
		TArray<TSharedPtr<FFunctionModel>> Functions;

		virtual FJsonObject ToJson() const override;
	};
} // namespace Kds::DocGen::Models
