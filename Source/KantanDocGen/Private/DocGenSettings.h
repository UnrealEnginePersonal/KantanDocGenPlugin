// /***********************************************************************************
// *  File:             DocGenSettings.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "CoreMinimal.h"

#include "UObject/UnrealType.h"
#include "Engine/EngineTypes.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Actor.h"
#include "Misc/Paths.h"
#include "Misc/App.h"

#include "DocGenSettings.generated.h"

UENUM()
enum class EGenMethod : uint8
{
	Manual,
	Project,
	Plugins,
	ProjectAndPlugins
};

UENUM()
enum class EExportMethod : uint8
{
	XML,
	JSON
};

USTRUCT()
struct FKantanDocGenSettings
{
	GENERATED_BODY()

public:
	/** Generation method */
	UPROPERTY(EditAnywhere, Category = "Documentation")
	EGenMethod GenerationMethod = EGenMethod::Manual;

	/** Title of the generated documentation (also used for output dir : "OutputDirectory/DocumentationTitle/<doc>" */
	UPROPERTY(EditAnywhere, Category = "Documentation",
			  Meta = (EditCondition = "GenerationMethod==EGenMethod::Manual", EditConditionHides))
	FString DocumentationTitle;

	/** Exclude The Super Class from the documentation */
	UPROPERTY(EditAnywhere, Category = "Documentation",
			  Meta = (EditCondition = "GenerationMethod==EGenMethod::Manual", EditConditionHides))
	bool bExcludeSuperClass = true;

	/** List of C++ modules in which to search for blueprint-exposed classes to document. */
	UPROPERTY(EditAnywhere, Category = "Class Search",
			  Meta = (Tooltip = "Raw module names (Do not prefix with '/Script').",
					  EditCondition = "GenerationMethod==EGenMethod::Manual", EditConditionHides))
	TArray<FName> NativeModules;

	/** List of paths in which to search for blueprints to document. */
	UPROPERTY(EditAnywhere, Category = "Class Search",
			  Meta = (ContentDir, EditCondition = "GenerationMethod==EGenMethod::Manual", EditConditionHides))
	TArray<FDirectoryPath> ContentPaths;

	UPROPERTY(EditAnywhere, Category = "Output")
	EExportMethod ExportMethod = EExportMethod::XML;

	UPROPERTY(EditAnywhere, Category = "Output")
	FDirectoryPath OutputDirectory;

	UPROPERTY(EditAnywhere, Category = "Class Search", AdvancedDisplay)
	TSubclassOf<UObject> BlueprintContextClass;

	UPROPERTY(EditAnywhere, Category = "Output")
	bool bCleanOutputDirectory;	
	TMap<FName, bool> AvailablePluginsAndModules;

public:
	FKantanDocGenSettings()
	{
		BlueprintContextClass = AActor::StaticClass();
		bExcludeSuperClass = true;
		bCleanOutputDirectory = false;
		ExportMethod = EExportMethod::XML;
	}

	TArray<FName> GetEnabledModules() const
	{
		TArray<FName> EnabledModules;
		for (const auto& PluginModule : AvailablePluginsAndModules)
		{
			if (PluginModule.Value)
			{
				EnabledModules.Add(PluginModule.Key);
			}
		}
		return EnabledModules;
	}

	bool HasAnySources() const
	{
		return GenerationMethod != EGenMethod::Manual || NativeModules.Num() > 0 || ContentPaths.Num() > 0;
	}
};

UCLASS(Config = EditorPerProjectUserSettings)
class UKantanDocGenSettingsObject : public UObject
{
	GENERATED_BODY()

public:
	static UKantanDocGenSettingsObject* Get()
	{
		static bool bInitialized = false;

		// This is a singleton, use default object
		const auto DefaultSettings = GetMutableDefault<UKantanDocGenSettingsObject>();

		if (!bInitialized)
		{
			InitDefaults(DefaultSettings);

			bInitialized = true;
		}

		return DefaultSettings;
	}

	static void InitDefaults(UKantanDocGenSettingsObject* CDO);

public:
	UPROPERTY(EditAnywhere, Config, Category = "Kantan DocGen", Meta = (ShowOnlyInnerProperties))
	FKantanDocGenSettings Settings;
};
