// /***********************************************************************************
// *  File:             Writer.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "CoreMinimal.h"
#include "Models/JsonModel.h"

namespace KantanDocGen::Json
{	
	struct FDoc
	{
	public:
		explicit FDoc(const FString& InTitle);

	private:
		const FString Title;
		TArray<Models::FClassModel> Classes;
	};

	class FWriter
	{
	public:
		FWriter(const FString& InDocsTitle, const FString& InOutputDir);
		~FWriter();
		
		void AddClass(UClass* Class, const TSharedPtr<Models::FClassModel>& ClassModel);
		bool Contains(UClass* Class) const;
		TSharedPtr<Models::FClassModel> FindClassChecked(UClass* Class) const;
		
		FString GetDocsTitle() const { return DocsTitle; }
		FString GetOutputDir() const { return OutputDir; }
		void Save(const FString& string);

	private:
		FString DocsTitle;
		FString OutputDir;

		TMap<TWeakObjectPtr<UClass>, TSharedPtr<Models::FClassModel>> ClassDocsMap;
	};
}