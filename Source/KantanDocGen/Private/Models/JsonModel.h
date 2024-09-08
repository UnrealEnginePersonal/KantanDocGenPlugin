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

namespace KantanDocGen::Json::Models
{
	// Base Model for common properties
	struct FBaseModel
	{
	public:
		const FName Name;
		const FString Description;

		FBaseModel(const FName& InName, const FString& InDescription)
			: Name(InName), Description(InDescription)
		{
		}
	};

	// Property Model
	struct FPropertyModel : public FBaseModel
	{
	public:
		FPropertyModel(const FName& InDisplayName, const FString& InDescription) :
		FBaseModel(InDisplayName, InDescription){}
		FString RawDescription;
		FString RawShortDescription;
		FString Tooltip;
		FString CPPType;
		FString CPPForwardDeclaration;
		FString Type;
		FString DisplayType;
	};

	// Event Model
	struct FEventModel : public FBaseModel
	{
	public:
		const FString FullName;
		const FString Image;
		const bool bBlueprintCallable;
		const bool bBlueprintEvent;
		TArray<FPropertyModel> Parameters;

		FEventModel(const FName& InName, const FString& InFullName, const FString& InDescription,
		            const FString& InImage,
		            const bool bInBlueprintCallable, const bool bInBlueprintEvent)
			: FBaseModel(InName, InDescription), FullName(InFullName), Image(InImage),
			  bBlueprintCallable(bInBlueprintCallable), bBlueprintEvent(bInBlueprintEvent)
		{
		}

		void AddParameter(const FPropertyModel&& Parameter)
		{
			Parameters.Add(std::move(Parameter));
		}
	};

	// Function Model
	struct FFunctionModel : public FBaseModel
	{
	public:
		const FString FullName;
		const FString DisplayName;
		const FString Image;
		const bool bBlueprintCallable;
		const bool bBlueprintEvent;
		TArray<FPropertyModel> Parameters;

		FFunctionModel(const FName& InName,
			const FString& InFullName, const FString& InDescription,
		               const FString& InImage,
		               const bool bInBlueprintCallable, const bool bInBlueprintEvent)
			: FBaseModel(InName, InDescription), FullName(InFullName), DisplayName(InName.ToString()),
			  Image(InImage), bBlueprintCallable(bInBlueprintCallable), bBlueprintEvent(bInBlueprintEvent)
		{
		}

		void AddParameter(const FPropertyModel&& Parameter)
		{
			Parameters.Add(std::move(Parameter));
		}
	};

	// Class Model
	struct FClassModel : public FBaseModel
	{
		FClassModel(
			const FString& InDocsName,
			const FString& InId,
			const FName& InDisplayName,
		    const FString& InDescription)
			:
		FBaseModel(InDisplayName, InDescription),
		DocsName(InDocsName),
		Id(InId),
		IncludePath(),
		Properties(),
		Events(),
		Functions()
		{
		}

		void AddProperty(const FPropertyModel&& Property)
		{
			Properties.Add(std::move(Property));
		}

		void AddEvent(const FEventModel&& Event)
		{
			Events.Add(std::move(Event));
		}

		void AddFunction(const FFunctionModel&& Function)
		{
			Functions.Add(std::move(Function));
		}

		const FString DocsName;
		const FString Id;		

		FString IncludePath;
		FString ClassTree;		
		
		TArray<FPropertyModel> Properties;
		TArray<FEventModel> Events;
		TArray<FFunctionModel> Functions;
	};
}
