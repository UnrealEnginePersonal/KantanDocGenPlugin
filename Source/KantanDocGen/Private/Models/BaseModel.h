// /***********************************************************************************
// *  File:             BaseModel.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          08-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

namespace Kds::DocGen::Models
{
	/**
	 * @brief The base model for all models that need a name and description
	 */
	struct FBaseModel
	{
	public:
		/**
		 * @brief The name of the model
		 */
		const FName Name;

		/**
		 * @brief The description of the model
		 */
		const FString Description;

		/**
		 * @brief Constructor
		 * @param InName The name of the model
		 * @param InDescription The description of the model
		 */
		FBaseModel(const FName& InName, const FString& InDescription);

		/**
		 * @brief Destructor
		 */
		virtual ~FBaseModel();
	};
} // namespace Kds::DocGen::Models
