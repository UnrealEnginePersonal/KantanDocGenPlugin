// /***********************************************************************************
// *  File:             ContentPathEnumerator.cpp
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#include "ContentPathEnumerator.h"
#include "KantanDocGenLog.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/ARFilter.h"
#include "Engine/Blueprint.h"
#include "Animation/AnimBlueprint.h"

FContentPathEnumerator::FContentPathEnumerator(const FName& InPath)
{
	CurIndex = 0;

	Prepass(InPath);
}

void FContentPathEnumerator::Prepass(const FName& Path)
{
	auto& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>("AssetRegistry");
	auto& AssetRegistry = AssetRegistryModule.Get();

	FARFilter Filter;
	Filter.bRecursiveClasses = true;
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());

	// @TODO: Not sure about this, but for some reason was generating docs for 'AnimInstance' itself.
	Filter.RecursiveClassPathsExclusionSet.Add(UAnimBlueprint::StaticClass()->GetClassPathName());

	AssetRegistry.GetAssetsByPath(Path, AssetList, true);
	AssetRegistry.RunAssetsThroughFilter(AssetList, Filter);
}

UObject* FContentPathEnumerator::GetNext()
{
	UObject* Result = nullptr;

	while (CurIndex < AssetList.Num())
	{
		const auto& AssetData = AssetList[CurIndex];
		++CurIndex;

		if (auto Blueprint = Cast<UBlueprint>(AssetData.GetAsset()))
		{
			UE_LOG(LogKantanDocGen, Log, TEXT("Enumerating object '%s' at '%s'"), *Blueprint->GetName(),
			       *AssetData.GetObjectPathString());

			Result = Blueprint;
			break;
		}
	}

	return Result;
}

float FContentPathEnumerator::EstimateProgress() const
{
	return (float)CurIndex / (AssetList.Num() - 1);
}

int32 FContentPathEnumerator::EstimatedSize() const
{
	return AssetList.Num();
}
