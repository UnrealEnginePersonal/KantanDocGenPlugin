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

	/** List of C++ modules in which to search for blueprint-exposed classes to document. */
	UPROPERTY(EditAnywhere, Category = "Class Search",
		Meta = (Tooltip = "Raw module names (Do not prefix with '/Script').", EditCondition =
			"GenerationMethod==EGenMethod::Manual", EditConditionHides))
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

public:
	FKantanDocGenSettings()
	{
		BlueprintContextClass = AActor::StaticClass();
		bCleanOutputDirectory = false;
		ExportMethod = EExportMethod::XML;
	}

	bool HasAnySources() const
	{
		return
			GenerationMethod != EGenMethod::Manual ||
			NativeModules.Num() > 0 ||
			ContentPaths.Num() > 0;
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
		auto DefaultSettings = GetMutableDefault<UKantanDocGenSettingsObject>();

		if (!bInitialized)
		{
			InitDefaults(DefaultSettings);

			bInitialized = true;
		}

		return DefaultSettings;
	}

	static void InitDefaults(UKantanDocGenSettingsObject* CDO)
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
	}

public:
	UPROPERTY(EditAnywhere, Config, Category = "Kantan DocGen", Meta = (ShowOnlyInnerProperties))
	FKantanDocGenSettings Settings;
};
