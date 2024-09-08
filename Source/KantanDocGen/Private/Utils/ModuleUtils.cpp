#include "ModuleUtils.h"

#include "KantanDocGenLog.h"
#include "Interfaces/IPluginManager.h"
#include "Models/PluginModel.h"
#include "Modules/ModuleManager.h"

using namespace Kds::DocGen::Utils;
using namespace Kds::DocGen::Models;

static void Log(const FString& Message)
{
	UE_LOG(LogKantanDocGen, Log, TEXT("%s"), *Message);
}

FModuleUtils& FModuleUtils::Get()
{
	static FModuleUtils Instance; // Guaranteed to be created only once.
	return Instance;
}

TArray<FString> FModuleUtils::GetPluginNames()
{
	TArray<FString> PluginNames;
	for (const std::shared_ptr<FPluginModel>& Plugin : Plugins)
	{
		PluginNames.Add(Plugin->Name.ToString());
	}
	return PluginNames;
}
TArray<FName> FModuleUtils::GetPluginNamesAsFName()
{
	TArray<FName> PluginNames;
	for (const std::shared_ptr<FPluginModel>& Plugin : Plugins)
	{
		PluginNames.Add(Plugin->Name);
	}
	return PluginNames;
}
const TArray<std::shared_ptr<FPluginModel>>& FModuleUtils::GetPlugins() const
{
	return Plugins;
}
void FModuleUtils::Initialize()
{
	// Resolve out the paths for each module and add the cut-down into to our output array
	for (const TSharedRef<IPlugin>& Plugin : IPluginManager::Get().GetDiscoveredPlugins())
	{
		// Only get plugins that are a part of the game project
		if (Plugin->GetLoadedFrom() == EPluginLoadedFrom::Project)
		{
			Log(FString::Printf(TEXT("Adding modules from plugin plugin %s"), *Plugin->GetName()));

			FPluginModel PluginModel(FName(*Plugin->GetName()), Plugin->GetDescriptor().VersionName);
			for (const FModuleDescriptor& PluginModule : Plugin->GetDescriptor().Modules)
			{
				Log(FString::Printf(TEXT("Adding module %s from plugin %s"), *PluginModule.Name.ToString(),
									*Plugin->GetName()));
				PluginModel.AddModule(PluginModule);
			}
			Plugins.Add(std::make_shared<FPluginModel>(PluginModel));
		}
	}
}
