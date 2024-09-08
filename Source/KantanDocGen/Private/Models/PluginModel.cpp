#include "PluginModel.h"
namespace Kds::DocGen::Models
{

	FPluginModel::FPluginModel(const FName& InName, const FString& InDescription) :
		FBaseModel(InName, InDescription), ContentPath("/" + InName.ToString())
	{
	}
	
	
	const FDirectoryPath& FPluginModel::GetPath() const
	{
		return ContentPath;
	}
	
	const TArray<TSharedPtr<FModuleDescriptor>>& FPluginModel::GetModules() const
	{
		return Modules;
	}


	void FPluginModel::AddModule(const FModuleDescriptor& Module)
	{
		Modules.Add(MakeShared<FModuleDescriptor>(Module));
	}
	
	FJsonObject FPluginModel::ToJson() const
	{
		FJsonObject OutJson;
		OutJson.SetStringField("ModuleName", Name.ToString());
		OutJson.SetStringField("ModuleSourcePath", ContentPath.Path);
		OutJson.SetStringField("ModuleType", "Plugin");
		return OutJson;
	}
} // namespace Kds::DocGen::Models
