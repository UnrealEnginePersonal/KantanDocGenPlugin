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
} // namespace Kds::DocGen::Models
