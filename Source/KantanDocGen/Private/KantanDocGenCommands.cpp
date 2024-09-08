// /***********************************************************************************
// *  File:             KantanDocGenCommands.cpp
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#include "KantanDocGenCommands.h"

#define LOCTEXT_NAMESPACE "KantanDocGen"

void FKantanDocGenCommands::RegisterCommands()
{
	UI_COMMAND(ShowDocGenUI, "Kantan DocGen", "Shows the Kantan Doc Gen tab", EUserInterfaceActionType::Button,
	           FInputGesture());
	NameToCommandMap.Add(TEXT("ShowDocGenUI"), ShowDocGenUI);
}

#undef LOCTEXT_NAMESPACE
