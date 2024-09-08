﻿// /***********************************************************************************
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

		virtual ~FClassModel() override;
		
		void AddProperty(const FPropertyModel& Property);
		void AddEvent(const FEventModel& Event);
		void AddFunction(const FFunctionModel& Function);

		const FString DocsName;
		FString IncludePath;
		FString ClassTree;		
	private:
		const FString Id;
		
		TArray<TUniquePtr<FPropertyModel>> Properties;
		TArray<TUniquePtr<FEventModel>> Events;
		TArray<TUniquePtr<FFunctionModel>> Functions;
	};
} // namespace Kds::DocGen::Models