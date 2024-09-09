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
#include "KantanDocGenLog.h"


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
		FJsonObject JsonObject;		
		const FString ModuleName = this->GetDocsTitle();
		const FString ModuleSourcePath = "TODO";
		const FString ModuleType = "TODO";

		JsonObject.SetStringField("ModuleName", ModuleName);
		JsonObject.SetStringField("ModuleSourcePath", ModuleSourcePath);
		JsonObject.SetStringField("ModuleType", ModuleType);

		TArray<TSharedPtr<FJsonValue>> ClassesArray;
		
		for (TTuple<TWeakObjectPtr<UClass>, TSharedPtr<Models::FClassModel>> Pair : ClassDocsMap)
		{
			const TSharedPtr<Models::FClassModel>& ClassModel = Pair.Value;
			const TSharedPtr<FJsonObject> ClassJsonObject = MakeShared<FJsonObject>(ClassModel->ToJson());			
			ClassesArray.Add(MakeShared<FJsonValueObject>(ClassJsonObject));
		}
		
		// Adding classes array to the main JsonObject
		JsonObject.SetArrayField("Classes", ClassesArray);
		
		// Convert FJsonObject to a JSON string
		FString OutputString;
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(MakeShared<FJsonObject>(JsonObject), Writer);

		// Write JSON string to file
		if (const FString OutputPath = FPaths::Combine(OutputDir, ModuleName + ".json");
			FFileHelper::SaveStringToFile(OutputString, *OutputPath))
		{
			UE_LOG(LogKantanDocGen, Log, TEXT("Successfully saved JSON file: %s"), *OutputPath);
		}
		else
		{
			UE_LOG(LogKantanDocGen, Error, TEXT("Failed to save JSON file: %s"), *OutputPath);
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
