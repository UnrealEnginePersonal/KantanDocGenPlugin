// /***********************************************************************************
// *  File:             NativeModuleEnumerator.h
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

class FNativeModuleEnumerator final : public ISourceObjectEnumerator
{
public:
	explicit FNativeModuleEnumerator(const FName& InModuleName);

public:
	virtual UObject* GetNext() override;
	virtual float EstimateProgress() const override;
	virtual int32 EstimatedSize() const override;

protected:
	void PrePass(const FName& ModuleName);

protected:
	TArray<TWeakObjectPtr<>> ObjectList;
	int32 CurIndex;
};
