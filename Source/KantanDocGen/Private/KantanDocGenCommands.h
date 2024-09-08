// /***********************************************************************************
// *  File:             KantanDocGenCommands.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "CoreMinimal.h"

#include "Framework/Commands/Commands.h"

class FKantanDocGenCommands : public TCommands<FKantanDocGenCommands>
{
public:
	FKantanDocGenCommands()
		: TCommands<FKantanDocGenCommands>("KantanDocGen", // Context name for fast lookup
		                                   NSLOCTEXT("Contexts", "KantanDocGen", "Kantan Doc Gen"),
		                                   // Localized context name for displaying
		                                   NAME_None, // Parent
		                                   FAppStyle::GetAppStyleSetName() // Icon Style Set
		)
	{
	}

	/**
	 * Initialize commands
	 */
	virtual void RegisterCommands() override;

public:
	// Mode Switch
	TSharedPtr<FUICommandInfo> ShowDocGenUI;

	// Map
	TMap<FName, TSharedPtr<FUICommandInfo>> NameToCommandMap;
};
