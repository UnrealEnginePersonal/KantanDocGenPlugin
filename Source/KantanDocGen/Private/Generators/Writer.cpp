// /***********************************************************************************
// *  File:             Writer.cpp
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#include "Writer.h"

#include "Models/ClassModel.h"

namespace Kds::DocGen
{
	FDoc::FDoc(const FString& InModuleName, const FString& InModuleSourcePath, const FString& InModuleType) :
		ModuleName(InModuleName), ModuleSourcePath(InModuleSourcePath), ModuleType(InModuleType), Classes()
	{
		// Setup
	}

	FWriter::FWriter(const FString& InDocsTitle, const FString& InOutputDir) :
		DocsTitle(InDocsTitle), OutputDir(InOutputDir)
	{
		// Setup
	}


	TSharedPtr<Models::FClassModel> FWriter::FindClassChecked(UClass* Class) const
	{
		return ClassDocsMap.FindChecked(Class);
	}

	FString FWriter::GetDocsTitle() const
	{
		return DocsTitle;
	}

	FString FWriter::GetOutputDir() const
	{
		return OutputDir;
	}

	void FWriter::Save()
	{
		if (ClassDocsMap.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No classes to save"));
			return;
		}

		const FString ModuleName = ClassDocsMap.begin().Value()->DocsName;
		// TODO: Add module source path
		const FString ModuleSourcePath = "TODO";
		// TODO: Add module type
		const FString ModuleType = "TODO";

		for (TTuple<TWeakObjectPtr<UClass>, TSharedPtr<Models::FClassModel>> Pair : ClassDocsMap)
		{
			const TSharedPtr<Models::FClassModel>& ClassModel = Pair.Value;
			const UClass* Class = Pair.Key.Get();

			UE_LOG(LogTemp, Warning, TEXT("Saving Class: %s"), *Class->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Saving ClassModel: %s"), *ClassModel->Name.ToString());
		}
	}

	void FWriter::AddClass(UClass* Class, const TSharedPtr<Models::FClassModel>& ClassModel)
	{
		ClassDocsMap.Add(Class, ClassModel);
	}

	bool FWriter::Contains(UClass* Class) const
	{
		return ClassDocsMap.Contains(Class);
	}
} // namespace Kds::DocGen
