﻿// /***********************************************************************************
// *  File:             PluginsModel.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          08-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/


#pragma once
#include "BaseModel.h"
#include "ModuleDescriptor.h"
#include "UObject/SoftObjectPath.h"

namespace Kds::DocGen::Models
{
	struct FPluginModel final :  FBaseModel
	{
	public:
		FPluginModel(const FName& InName, const FString& InDescription, const FString& InBaseDiskDir);

		virtual ~FPluginModel() override = default;
		
	public:
		FString GetSourcePath() const;
		const FDirectoryPath& GetPath() const;
		const TArray<TSharedPtr<FModuleDescriptor>>& GetModules() const;
		
		void AddModule(const FModuleDescriptor& Module);

		virtual FJsonObject ToJson() const override;
		
	private:
		const FString BaseDiskDir;
		const FDirectoryPath ContentPath;
		TArray<TSharedPtr<FModuleDescriptor>> Modules;
	};
} // namespace Kds::DocGen::Models
