// /***********************************************************************************
// *  File:             CompositeEnumerator.h
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

template <typename TChildEnum>
class FCompositeEnumerator : public ISourceObjectEnumerator
{
public:
	FCompositeEnumerator(const TArray<FName>& InNames)
	{
		CurEnumIndex = 0;
		TotalSize = 0;
		Completed = 0;

		Prepass(InNames);
	}

public:
	virtual UObject* GetNext() override
	{
		while (CurEnumIndex < ChildEnumList.Num())
		{
			if (auto Obj = ChildEnumList[CurEnumIndex]->GetNext())
			{
				return Obj;
			}
			else
			{
				Completed += ChildEnumList[CurEnumIndex]->EstimatedSize();
				ChildEnumList[CurEnumIndex].Reset();
				++CurEnumIndex;
				continue;
			}
		}

		return nullptr;
	}

	virtual float EstimateProgress() const override
	{
		if (CurEnumIndex < ChildEnumList.Num())
		{
			return (float)(Completed + ChildEnumList[CurEnumIndex]->EstimateProgress() * ChildEnumList[CurEnumIndex]->
				EstimatedSize()) / TotalSize;
		}
		else
		{
			return 1.0f;
		}
	}

	virtual int32 EstimatedSize() const override
	{
		return TotalSize;
	}

protected:
	void Prepass(const TArray<FName>& Names)
	{
		for (auto Name : Names)
		{
			auto Child = MakeUnique<TChildEnum>(Name);
			TotalSize += Child->EstimatedSize();

			ChildEnumList.Add(MoveTemp(Child));
		}
	}

protected:
	TArray<TUniquePtr<ISourceObjectEnumerator>> ChildEnumList;
	int32 CurEnumIndex;
	int32 TotalSize;
	int32 Completed;
};
