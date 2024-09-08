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
		FClassModel(const FString& InDocsName, const FString& InId, const FName& InDisplayName,
					const FString& InDescription);
		~FClassModel() = default;
		
		void AddProperty(const FPropertyModel& Property);
		void AddEvent(const FEventModel& Event);
		void AddFunction(const FFunctionModel& Function);

		const FString DocsName;
		FString IncludePath;
		FString ClassTree;
		
		
		TArray<TSharedPtr<FPropertyModel>> Properties;
		TArray<TSharedPtr<FEventModel>> Events;
		TArray<TSharedPtr<FFunctionModel>> Functions;

		virtual FJsonObject ToJson() const override;
		
	private:
		const FString Id;
	};
} // namespace Kds::DocGen::Models
