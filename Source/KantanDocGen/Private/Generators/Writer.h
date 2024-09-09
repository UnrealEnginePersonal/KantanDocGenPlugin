// /***********************************************************************************
// *  File:             Writer.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

namespace Kds::DocGen
{
	namespace Models
	{
		struct FClassModel;
	}


	class FWriter
	{
	public:
		FWriter(const FString& InDocsTitle, const FString& InOutputDir);
		~FWriter() = default;

		void AddClass(UClass* Class, const TSharedPtr<Models::FClassModel>& ClassModel);
		void Save();

		bool Contains(UClass* Class) const;

		TSharedPtr<Models::FClassModel> FindClassChecked(UClass* Class) const;

		FString GetDocsTitle() const;
		FString GetOutputDir() const;

	private:
		const FString DocsTitle;
		const FString OutputDir;

		TMap<TWeakObjectPtr<UClass>, TSharedPtr<Models::FClassModel>> ClassDocsMap;
	};
} // namespace Kds::DocGen
