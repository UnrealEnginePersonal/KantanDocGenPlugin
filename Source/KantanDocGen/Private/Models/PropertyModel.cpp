﻿// /***********************************************************************************
// *  File:             JsonModel.cpp
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#include "PropertyModel.h"

namespace Kds::DocGen::Models
{
	FPropertyModel::FPropertyModel(const FName& InDisplayName, const FString& InDescription) :
		FBaseModel(InDisplayName, InDescription)
	{
	}
} // namespace Kds::DocGen::Models