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

#include "KantanDocGenLog.h"

#include "Models/JsonModel.h"

namespace KantanDocGen::Json
{
	FDoc::FDoc(const FString& InTitle)
		: Title(InTitle)
		, Classes()
	{
	}

	FWriter::FWriter(const FString& InDocsTitle, const FString& InOutputDir)
		: DocsTitle(InDocsTitle)
		, OutputDir(InOutputDir)
		, ClassDocsMap()
	{
	}

	FWriter::~FWriter()
	{
		//Cleanup
	}

	TSharedPtr<Models::FClassModel> FWriter::FindClassChecked(UClass* Class) const
	{
		return ClassDocsMap.FindChecked(Class);
	}

	void FWriter::Save(const FString& string)
	{
		
		for(TTuple<TWeakObjectPtr<UClass>, TSharedPtr<Models::FClassModel>> Pair : ClassDocsMap)
        {
           const TSharedPtr<Models::FClassModel>& ClassModel = Pair.Value;
        }
	}

	void FWriter::AddClass(UClass* Class, const TSharedPtr<Models::FClassModel>& ClassModel)
	{
		UE_LOG(LogKantanDocGen, Log, TEXT("Adding class %s to writer"), *Class->GetName());
		ClassDocsMap.Add(Class, ClassModel);
	}

	bool FWriter::Contains(UClass* Class) const
	{
		return ClassDocsMap.Contains(Class);
	}
}
