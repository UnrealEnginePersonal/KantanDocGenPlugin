// /***********************************************************************************
// *  File:             SKantanDocGenWidget.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "CoreMinimal.h"

#include "Widgets/SCompoundWidget.h"

class SKantanDocGenWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SKantanDocGenWidget)
		{
		}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	
	FReply OnGenerateDocs();

protected:

private:
	static bool ValidateSettingsForGeneration();
	static TSharedRef<SWidget> GeneratePluginsAndModulesCheckboxes(TMap<FName, bool>& AvailablePluginsAndModules);
};
