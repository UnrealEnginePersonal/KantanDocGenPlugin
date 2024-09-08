// /***********************************************************************************
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
		FBaseModel(InDisplayName, InDescription), DisplayName(InDisplayName)
	{
	}
	FJsonObject FPropertyModel::ToJson() const
	{
		FJsonObject OutJson;

		OutJson.SetStringField("Name", this->Name.ToString());
		OutJson.SetStringField("DisplayName", this->DisplayName.ToString());
		OutJson.SetStringField("RawDescription", this->RawDescription);
		OutJson.SetStringField("RawShortDescription", this->RawShortDescription);
		OutJson.SetStringField("Tooltip", this->Tooltip);
		OutJson.SetStringField("Description", this->Description);
		OutJson.SetStringField("CPPType", this->CPPType);
		OutJson.SetStringField("CPPTypeForwardDeclaration", this->CPPTypeForwardDeclaration);
		OutJson.SetStringField("Type", this->Type);
		OutJson.SetStringField("DisplayType", this->DisplayType);

		return OutJson;
	}
} // namespace Kds::DocGen::Models
