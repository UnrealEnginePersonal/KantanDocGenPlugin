// /***********************************************************************************
// *  File:             ISourceObjectEnumerator.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "CoreMinimal.h"

class ISourceObjectEnumerator
{
public:
	virtual UObject* GetNext() = 0;
	virtual float EstimateProgress() const = 0;
	virtual int32 EstimatedSize() const = 0;

	virtual ~ISourceObjectEnumerator()
	{
	}
};
