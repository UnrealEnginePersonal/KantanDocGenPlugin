#include "DocGenSettings.h"

#include "Models/FunctionModel.h"
#include "Utils/ModuleUtils.h"

using namespace Kds::DocGen::Utils;
using namespace Kds::DocGen::Models;

void UKantanDocGenSettingsObject::InitDefaults(UKantanDocGenSettingsObject* CDO)
{
	if (CDO->Settings.DocumentationTitle.IsEmpty())
	{
		CDO->Settings.DocumentationTitle = FApp::GetProjectName();
	}

	if (CDO->Settings.OutputDirectory.Path.IsEmpty())
	{
		CDO->Settings.OutputDirectory.Path = FPaths::ProjectSavedDir() / TEXT("KantanDocGen");
	}

	if (CDO->Settings.BlueprintContextClass == nullptr)
	{
		CDO->Settings.BlueprintContextClass = AActor::StaticClass();
	}

	CDO->Settings.AvailablePluginsAndModules.Empty();

	for (const auto Plugin : FModuleUtils::Get().GetPlugins())
	{
		for(const auto Module : Plugin->GetModules())
		{
			CDO->Settings.AvailablePluginsAndModules.Add(Module->Name, false);
		}
	}
}