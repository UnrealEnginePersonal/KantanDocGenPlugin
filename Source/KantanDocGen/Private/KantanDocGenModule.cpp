// /***********************************************************************************
// *  File:             KantanDocGenModule.cpp
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#include "KantanDocGenModule.h"
#include "KantanDocGenLog.h"
#include "KantanDocGenCommands.h"
#include "DocGenSettings.h"
#include "DocGenTaskProcessor.h"
#include "UI/SKantanDocGenWidget.h"
#include "Utils/ModuleUtils.h"

#include "HAL/IConsoleManager.h"
#include "Interfaces/IMainFrameModule.h"
#include "LevelEditor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/RunnableThread.h"



#define LOCTEXT_NAMESPACE "KantanDocGen"

IMPLEMENT_MODULE(FKantanDocGenModule, KantanDocGen)

DEFINE_LOG_CATEGORY(LogKantanDocGen);
using namespace Kds::DocGen::Utils;

void FKantanDocGenModule::StartupModule()
{
	// Create command list
	UICommands = MakeShared<FUICommandList>();

	FKantanDocGenCommands::Register();

	// Map commands
	FUIAction ShowDocGenUI_UIAction(FExecuteAction::CreateRaw(this, &FKantanDocGenModule::ShowDocGenUI),
	                                FCanExecuteAction::CreateLambda(
		                                []
		                                {
			                                return true;
		                                }));

	auto CmdInfo = FKantanDocGenCommands::Get().ShowDocGenUI;
	UICommands->MapAction(CmdInfo, ShowDocGenUI_UIAction);

	// Setup menu extension
	auto AddMenuExtension = [](FMenuBuilder& MenuBuilder)
	{
		MenuBuilder.AddMenuEntry(FKantanDocGenCommands::Get().ShowDocGenUI);
	};

	auto& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedRef<FExtender> MenuExtender(new FExtender());
	MenuExtender->AddMenuExtension(TEXT("FileProject"), EExtensionHook::After, UICommands.ToSharedRef(),
	                               FMenuExtensionDelegate::CreateLambda(AddMenuExtension));
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	FModuleUtils::Get();
}

void FKantanDocGenModule::ShutdownModule()
{
	FKantanDocGenCommands::Unregister();
}

// @TODO: Idea was to allow quoted values containing spaces, but this isn't possible since the initial console string has
// already been split by whitespace, ignoring quotes...
inline bool MatchPotentiallyQuoted(const TCHAR* Stream, const TCHAR* Match, FString& Value)
{
	while ((*Stream == ' ') || (*Stream == 9))
	{
		Stream++;
	}

	if (FCString::Strnicmp(Stream, Match, FCString::Strlen(Match)) == 0)
	{
		Stream += FCString::Strlen(Match);

		return FParse::Token(Stream, Value, false);
	}

	return false;
}

void FKantanDocGenModule::GenerateDocs(const FKantanDocGenSettings& Settings)
{
	if (!Processor.IsValid())
	{
		Processor = MakeUnique<FDocGenTaskProcessor>();
	}
	else if (Processor->IsRunning())
	{
		UE_LOG(LogKantanDocGen, Warning, TEXT("Doc gen processor is already running. Ignoring request."));
		return;
	}

	Processor->QueueTask(Settings);
	FRunnableThread::Create(Processor.Get(), TEXT("KantanDocGenProcessorThread"), 0, TPri_BelowNormal);
}

void FKantanDocGenModule::ShowDocGenUI()
{
	const FText WindowTitle = LOCTEXT("DocGenWindowTitle", "Kantan Doc Gen");

	TSharedPtr<SWindow> Window = SNew(SWindow)
		.Title(WindowTitle)
		.MinWidth(400.0f)
		.MinHeight(300.0f)
		.MaxHeight(600.0f)
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		.SizingRule(ESizingRule::Autosized);

	TSharedRef<SWidget> DocGenContent = SNew(SKantanDocGenWidget);
	Window->SetContent(DocGenContent);

	IMainFrameModule& MainFrame = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
	TSharedPtr<SWindow> ParentWindow = MainFrame.GetParentWindow();

	if (ParentWindow.IsValid())
	{
		FSlateApplication::Get().AddModalWindow(Window.ToSharedRef(), ParentWindow.ToSharedRef());

		auto Settings = UKantanDocGenSettingsObject::Get();
		Settings->SaveConfig();
	}
	else
	{
		FSlateApplication::Get().AddWindow(Window.ToSharedRef());
	}
}
#undef LOCTEXT_NAMESPACE
