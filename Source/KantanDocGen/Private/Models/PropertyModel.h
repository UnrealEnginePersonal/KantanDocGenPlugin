// /***********************************************************************************
// *  File:             JsonModel.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once
#include "BaseModel.h"

namespace Kds::DocGen::Models
{
	/**
	 * @brief The model for a property
	 */
	struct FPropertyModel final : FBaseModel
	{
	public:
		FString RawDescription;
		FString RawShortDescription;
		FString Tooltip;
		FString CPPType;
		FString CPPForwardDeclaration;
		FString Type;
		FString DisplayType;

	public:
		/**
		 * @brief  Constructor
		 * @param InDisplayName The display name of the property
		 * @param InDescription The description of the property
		 * @see FBaseModel
		 */
		FPropertyModel(const FName& InDisplayName, const FString& InDescription);

		/**
		 * @brief Destructor
		 */
		virtual ~FPropertyModel() override = default;
	};
} // namespace Kds::DocGen::Models
