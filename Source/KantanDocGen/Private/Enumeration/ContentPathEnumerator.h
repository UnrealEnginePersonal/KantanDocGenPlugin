// /***********************************************************************************
// *  File:             ContentPathEnumerator.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "CoreMinimal.h"

#include "ISourceObjectEnumerator.h"

class FContentPathEnumerator final : public ISourceObjectEnumerator
{
public:
	explicit FContentPathEnumerator(const FName& InPath);

public:
	virtual UObject* GetNext() override;
	virtual float EstimateProgress() const override;
	virtual int32 EstimatedSize() const override;

protected:
	void PrePass(const FName& Path);

protected:
	TArray<FAssetData> AssetList;
	int32 CurIndex;
};
