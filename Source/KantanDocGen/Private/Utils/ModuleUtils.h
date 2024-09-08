// /***********************************************************************************
// *  File:             ModuleUtils.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          08-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once
#include <memory>
#include "Models/PluginModel.h"
namespace Kds::DocGen
{
	namespace Utils
	{
		class FModuleUtils
		{
		public:
			static FModuleUtils& Get();

			TArray<FString> GetPluginNames();
			TArray<FName> GetPluginNamesAsFName();

			const TArray<std::shared_ptr<Models::FPluginModel>>& GetPlugins() const;
		private:
			// Make the constructor private to prevent external instantiation
			FModuleUtils() { Initialize(); }

			void Initialize();
			TArray<std::shared_ptr<Models::FPluginModel>> Plugins;
		};
	} // namespace Utils
} // namespace Kds::DocGen
