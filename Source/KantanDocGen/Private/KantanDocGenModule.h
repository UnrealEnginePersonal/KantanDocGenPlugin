// /***********************************************************************************
// *  File:             KantanDocGenModule.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleManager.h"
#include "DocGenTaskProcessor.h" // TUniquePtr seems to need full definition...

class FUICommandList;

/*
Module implementation
*/
class FKantanDocGenModule : public FDefaultGameModuleImpl
{
public:
	FKantanDocGenModule()
	{
	}

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void GenerateDocs(const FKantanDocGenSettings& Settings);

protected:
	void ShowDocGenUI();

protected:
	TUniquePtr<FDocGenTaskProcessor> Processor;

	TSharedPtr<FUICommandList> UICommands;
};
