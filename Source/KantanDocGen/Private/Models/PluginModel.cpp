#include "PluginModel.h"
namespace Kds::DocGen::Models
{

	FPluginModel::FPluginModel(const FName& InName, const FString& InDescription, const FString& InBaseDiskDir):
		FBaseModel(InName, InDescription), ContentPath("/" + InName.ToString()), BaseDiskDir(InBaseDiskDir)
	{
	}


	FString FPluginModel::GetSourcePath() const
	{
		static const FString SourceFolder = "Source";
		if(BaseDiskDir.EndsWith("/"))
		{
			return BaseDiskDir + SourceFolder;
		}

		return BaseDiskDir + "/" + SourceFolder;
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
