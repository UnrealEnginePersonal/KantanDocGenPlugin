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

	auto DetailView = PropertyEditorModule.CreateDetailView(DetailArgs);

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot().AutoHeight()
		[
			DetailView
		]

		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("GenButtonLabel", "Generate Docs"))
				.IsEnabled(this, &SKantanDocGenWidget::ValidateSettingsForGeneration)
				.OnClicked(this, &SKantanDocGenWidget::OnGenerateDocs)
			]
		]
	];

	auto Settings = UKantanDocGenSettingsObject::Get();
	DetailView->SetObject(Settings);
}

bool SKantanDocGenWidget::ValidateSettingsForGeneration() const
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

#undef LOCTEXT_NAMESPACE
