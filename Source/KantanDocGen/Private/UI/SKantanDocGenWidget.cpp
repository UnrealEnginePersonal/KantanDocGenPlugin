// /***********************************************************************************
// *  File:             SKantanDocGenWidget.cpp
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#include "SKantanDocGenWidget.h"
#include "DocGenSettings.h"
#include "KantanDocGenModule.h"

#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "KantanDocGen"

void SKantanDocGenWidget::Construct(const SKantanDocGenWidget::FArguments& InArgs)
{
	auto& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailArgs;
	DetailArgs.bUpdatesFromSelection = false;
	DetailArgs.bLockable = false;
	DetailArgs.NameAreaSettings = FDetailsViewArgs::ComponentsAndActorsUseNameArea;
	DetailArgs.bCustomNameAreaLocation = false;
	DetailArgs.bCustomFilterAreaLocation = true;
	DetailArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;

	const auto DetailView = PropertyEditorModule.CreateDetailView(DetailArgs);
	const auto Settings = UKantanDocGenSettingsObject::Get();
	
	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot().AutoHeight()
		[
			DetailView
		]

		// Checkboxes for AvailablePluginsAndModules map
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("PluginsAndModulesLabel", "Available Plugins and Modules"))
		]		
		+ SVerticalBox::Slot().AutoHeight()
		[
			GeneratePluginsAndModulesCheckboxes(Settings->Settings.AvailablePluginsAndModules)
		]

		// Generate button
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("GenButtonLabel", "Generate Docs"))
				.IsEnabled(&SKantanDocGenWidget::ValidateSettingsForGeneration)
				.OnClicked(this, &SKantanDocGenWidget::OnGenerateDocs)
			]
		]
	];
	DetailView->SetObject(Settings);
}


FReply SKantanDocGenWidget::OnGenerateDocs()
{
	auto& Module = FModuleManager::LoadModuleChecked<FKantanDocGenModule>(TEXT("KantanDocGen"));
	Module.GenerateDocs(
		UKantanDocGenSettingsObject::Get()->Settings
	);

	TSharedRef<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AsShared()).ToSharedRef();
	FSlateApplication::Get().RequestDestroyWindow(ParentWindow);

	return FReply::Handled();
}


//****STATIC FUNCTION****//

bool SKantanDocGenWidget::ValidateSettingsForGeneration()
{
	const auto& Settings = UKantanDocGenSettingsObject::Get()->Settings;

	if (Settings.DocumentationTitle.IsEmpty())
	{
		return false;
	}

	if (!Settings.HasAnySources())
	{
		return false;
	}

	if (Settings.BlueprintContextClass == nullptr)
	{
		return false;
	}

	return true;
}

// Function to generate checkboxes for the AvailablePluginsAndModules map
TSharedRef<SWidget> SKantanDocGenWidget::GeneratePluginsAndModulesCheckboxes(TMap<FName, bool>& AvailablePluginsAndModules)
{
	TSharedRef<SVerticalBox> CheckBoxList = SNew(SVerticalBox);

	for (auto& PluginModule : AvailablePluginsAndModules)
	{
		FName PluginModuleName = PluginModule.Key;
		bool& bIsEnabled = PluginModule.Value;

		// Create a checkbox for each plugin/module
		CheckBoxList->AddSlot().AutoHeight()
		[
			SNew(SCheckBox)
			.IsChecked_Lambda([&bIsEnabled]() -> ECheckBoxState
			{
				return bIsEnabled ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
			})
			.OnCheckStateChanged_Lambda([&bIsEnabled](ECheckBoxState NewState)
			{
				bIsEnabled = (NewState == ECheckBoxState::Checked);
			})
			[
				SNew(STextBlock).Text(FText::FromName(PluginModuleName))
			]
		];
	}

	return CheckBoxList;
}

#undef LOCTEXT_NAMESPACE
