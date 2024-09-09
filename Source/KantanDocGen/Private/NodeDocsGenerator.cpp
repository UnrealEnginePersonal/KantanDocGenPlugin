// /***********************************************************************************
// *  File:             NodeDocsGenerator.cpp
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "NodeDocsGenerator.h"
#include "KantanDocGenLog.h"
#include "SGraphNode.h"
#include "SGraphPanel.h"
#include "NodeFactory.h"
#include "EdGraphSchema_K2.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintFunctionNodeSpawner.h"
#include "BlueprintBoundNodeSpawner.h"
#include "BlueprintComponentNodeSpawner.h"
#include "BlueprintEventNodeSpawner.h"
#include "BlueprintBoundEventNodeSpawner.h"
#include "K2Node_DynamicCast.h"
#include "K2Node_Message.h"
#include "XmlFile.h"
#include "Slate/WidgetRenderer.h"
#include "Engine/TextureRenderTarget2D.h"
#include "TextureResource.h"
#include "ThreadingHelpers.h"
#include "Stats/StatsMisc.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"
#include "AnimGraphNode_Base.h"

#include "Models/ClassModel.h"
#include "Models/FunctionModel.h"

#include "Generators/Writer.h"

#include "Utils/ClassUtils.h"
#include "Models/PropertyModel.h"
#include "Utils/FunctionUtils.h"

using namespace Kds::DocGen;
using namespace Kds::DocGen::Models;
using namespace Kds::DocGen::Utils;

FNodeDocsGenerator::FNodeProcessingState::FNodeProcessingState() :
	ClassModel(nullptr), ClassDocsPath(), RelImageBasePath(), ImageFilename()
{
	UE_LOG(LogKantanDocGen, Log, TEXT("NodeProcessingState created."));
}

FNodeDocsGenerator::FNodeDocsGenerator() {}

FNodeDocsGenerator::~FNodeDocsGenerator()
{
	CleanUp();
}

bool FNodeDocsGenerator::GT_Init(const FString& InDocsTitle, const FString& InOutputDir,
								 UClass* BlueprintContextClass /*= AActor::StaticClass()*/
)
{
	/*Writer = MakeShareable<FWriter>(new FWriter());	*/

	DummyBP = CastChecked<UBlueprint>(FKismetEditorUtilities::CreateBlueprint(
		BlueprintContextClass, GetTransientPackage(), NAME_None, BPTYPE_Normal, UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass(), NAME_None));

	if (!DummyBP.IsValid())
	{
		return false;
	}

	Graph = FBlueprintEditorUtils::CreateNewGraph(DummyBP.Get(), TEXT("TempoGraph"), UEdGraph::StaticClass(),
												  UEdGraphSchema_K2::StaticClass());

	DummyBP->AddToRoot();
	Graph->AddToRoot();

	GraphPanel = SNew(SGraphPanel).GraphObj(Graph.Get());

	// We want full detail for rendering, passing a super-high zoom value will guarantee the highest LOD.
	GraphPanel->RestoreViewSettings(FVector2D(0, 0), 10.0f);

	this->Writer = MakeShareable(new FWriter(InDocsTitle, InOutputDir));
	return true;
}

UK2Node* FNodeDocsGenerator::GT_DocumentSimpleObject(UObject* SourceObject, FNodeProcessingState& OutState)
{
	UE_LOG(LogKantanDocGen, Log, TEXT("Documenting object %s..."), *GetNameSafe(SourceObject));

	if (const UEnum* Enum = Cast<UEnum>(SourceObject))
	{
		UE_LOG(LogKantanDocGen, Log, TEXT("Documenting enum %s..."), *Enum->GetName());

		UpdateIndexDocWithEnum(Writer.Get(), Enum);

		return nullptr;
	}

	if (const UScriptStruct* Struct = Cast<UScriptStruct>(SourceObject))
	{
		UE_LOG(LogKantanDocGen, Log, TEXT("Documenting struct %s..."), *Struct->GetName());

		UpdateIndexDocWithStruct(Writer.Get(), Struct);

		return nullptr;
	}

	// InitializeForSpawner supports "nullptr" spawner for simple classes (handled gracefully in IsSpawnerDocumentable)
	return GT_InitializeForSpawner(nullptr, SourceObject, OutState);
}

UK2Node* FNodeDocsGenerator::GT_InitializeForSpawner(UBlueprintNodeSpawner* Spawner, UObject* SourceObject, const bool bExcludeSuper,
													 FNodeProcessingState& OutState)
{
	UK2Node* K2NodeInst = nullptr;
	const bool bIsDocumentable = CanBeDocumented(Spawner, SourceObject);

	// Try to create a node from the spawner if it is documentable,
	// This will be a null pointer if the spawner is not documentable
	if (bIsDocumentable)
	{
		const auto NodeInst = Spawner->Invoke(Graph.Get(), IBlueprintNodeBinder::FBindingSet{}, FVector2D(0, 0));
		// Currently Blueprint nodes only
		K2NodeInst = Cast<UK2Node>(NodeInst);
		if (K2NodeInst == nullptr)
		{
			UE_LOG(LogKantanDocGen, Warning,
				   TEXT("Failed to create node from spawner of class %s with node class %s (Object %s)."),
				   *Spawner->GetClass()->GetName(), Spawner->NodeClass ? *Spawner->NodeClass->GetName() : TEXT("None"),
				   *GetNameSafe(SourceObject));
			return nullptr;
		}
	}

	auto AssociatedClass = GetAssociatedClass(K2NodeInst, SourceObject);

	if (!Writer->Contains(AssociatedClass))
	{
		const TSharedPtr<Models::FClassModel> GeneratedModel = InitClassDocXml(AssociatedClass,bExcludeSuper); 
		Writer->AddClass(AssociatedClass, GeneratedModel);
		UpdateIndexDocWithClass(Writer.Get(), AssociatedClass, SourceObject);
	}

	if (bIsDocumentable)
	{
		OutState = FNodeProcessingState();
		OutState.ClassModel = Writer->FindClassChecked(AssociatedClass);
		OutState.ClassDocsPath = Writer->GetOutputDir() / FClassUtils::GetClassNameString(AssociatedClass);
	}
	return K2NodeInst;
}

bool FNodeDocsGenerator::GT_Finalize(const FString& OutputPath)
{
	UE_LOG(LogKantanDocGen, Log, TEXT("Generating documentation..."));

	if (!SaveClassDocXml(OutputPath))
	{
		return false;
	}

	if (!SaveIndexXml(OutputPath))
	{
		return false;
	}

	return true;
}

void FNodeDocsGenerator::CleanUp()
{
	if (GraphPanel.IsValid())
	{
		GraphPanel.Reset();
	}

	if (DummyBP.IsValid())
	{
		DummyBP->RemoveFromRoot();
		DummyBP.Reset();
	}
	if (Graph.IsValid())
	{
		Graph->RemoveFromRoot();
		Graph.Reset();
	}
}

bool FNodeDocsGenerator::GenerateNodeImage(UEdGraphNode* Node, FNodeProcessingState& State)
{
	SCOPE_SECONDS_COUNTER(GenerateNodeImageTime);

	AdjustNodeForSnapshot(Node);

	const FString NodeName = GetNodeDocId(Node);

	bool bSuccess = DocGenThreads::RunOnGameThreadRetVal(
		[&]
		{
			auto NodeWidget = FNodeFactory::CreateNodeWidget(Node);
			NodeWidget->SetOwner(GraphPanel.ToSharedRef());

			constexpr bool bUseGammaCorrection = false;
			FWidgetRenderer Renderer(bUseGammaCorrection);
			Renderer.SetIsPrepassNeeded(true);
			auto RenderTarget = Renderer.DrawWidget(NodeWidget.ToSharedRef(), FVector2D(1024.0f, 1024.0f));

			auto Desired = NodeWidget->GetDesiredSize();

			FTextureRenderTargetResource* RTResource = RenderTarget->GameThread_GetRenderTargetResource();
			const FIntRect Rect = FIntRect(0, 0, static_cast<int32>(Desired.X), static_cast<int32>(Desired.Y));
			FReadSurfaceDataFlags ReadPixelFlags(RCM_UNorm);
			ReadPixelFlags.SetLinearToGamma(true); // @TODO: is this gamma correction, or something else?

			TArray<FColor> Pixels;
			if (RTResource->ReadPixels(Pixels, ReadPixelFlags, Rect) == false)
			{
				UE_LOG(LogKantanDocGen, Warning, TEXT("Failed to read pixels for node image."));
				return false;
			}

			IImageWrapperModule& ImageWrapperModule =
				FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

			TArray<uint8> CompressedImage;

			const int32 X = static_cast<int32>(Desired.X);
			if (const int32 Y = static_cast<int32>(Desired.Y);
				!ImageWrapper->SetRaw(Pixels.GetData(), X * Y * sizeof(FColor), X, Y, ERGBFormat::BGRA, 8))
			{
				UE_LOG(LogKantanDocGen, Error, TEXT("Failed to set raw pixels for node %s."), *NodeName);
				return false;
			}

			CompressedImage = ImageWrapper->GetCompressed(90);

			State.RelImageBasePath = TEXT("../img");
			FString ImageBasePath = State.ClassDocsPath / TEXT("img"); // State.RelImageBasePath;
			FString ImgFilename = FString::Printf(TEXT("nd_img_%s.png"), *NodeName);
			FString ScreenshotSaveName = ImageBasePath / ImgFilename;

			if (!FFileHelper::SaveArrayToFile(CompressedImage, *ScreenshotSaveName))
			{
				UE_LOG(LogKantanDocGen, Error, TEXT("Failed to save %s."), *ScreenshotSaveName);
				return false;
			}
			State.ImageFilename = ImgFilename;
			return true;
		});
	return bSuccess;
}

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
inline FString WrapAsCDATA(const FString& InString)
{
	return TEXT("<![CDATA[") + InString + TEXT("]]>");
}

inline FXmlNode* AppendChild(FXmlNode* Parent, const FString& Name)
{
	Parent->AppendChildNode(Name, FString());
	return Parent->GetChildrenNodes().Last();
}

inline FXmlNode* AppendChildRaw(FXmlNode* Parent, const FString& Name, const FString& TextContent)
{
	Parent->AppendChildNode(Name, TextContent);
	return Parent->GetChildrenNodes().Last();
}

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
inline FXmlNode* AppendChildCDATA(FXmlNode* Parent, const FString& Name, const FString& TextContent)
{
	Parent->AppendChildNode(Name, WrapAsCDATA(TextContent));
	return Parent->GetChildrenNodes().Last();
}

// For K2 pins only!
bool ExtractPinInformation(const UEdGraphPin* Pin, FString& OutName, FString& OutType, FString& OutDescription)
{
	FString Tooltip;
	Pin->GetOwningNode()->GetPinHoverText(*Pin, Tooltip);

	if (!Tooltip.IsEmpty())
	{
		// @NOTE: This is based on the formatting in UEdGraphSchema_K2::ConstructBasicPinTooltip.
		// If that is changed, this will fail!

		auto TooltipPtr = *Tooltip;

		// Parse name line
		FParse::Line(&TooltipPtr, OutName);
		// Parse type line
		FParse::Line(&TooltipPtr, OutType);

		// Currently there is an empty line here, but FParse::Line seems to gobble up empty lines as part of the
		// previous call. Anyway, attempting here to deal with this generically in case that weird behaviour changes.
		while (*TooltipPtr == TEXT('\n'))
		{
			FString Buf;
			FParse::Line(&TooltipPtr, Buf);
		}

		// What remains is the description
		OutDescription = TooltipPtr;
	}

	// @NOTE: Currently overwriting the name and type as suspect this is more robust to future engine changes.

	OutName = Pin->GetDisplayName().ToString();
	if (OutName.IsEmpty() && Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
	{
		OutName = Pin->Direction == EEdGraphPinDirection::EGPD_Input ? TEXT("In") : TEXT("Out");
	}

	OutType = UEdGraphSchema_K2::TypeToText(Pin->PinType).ToString();

	return true;
}

inline static FPropertyModel CreatePropertyModel(const FProperty* Prop)
{
	const FName DisplayName = FName(*Prop->GetDisplayNameText().ToString());
	const FString Description = Prop->GetToolTipText().ToString();
	const auto CPPType = Prop->GetCPPType();
	const auto CPPForwardDeclare = Prop->GetCPPTypeForwardDeclaration();
	const auto DisplayType = Prop->GetCPPType();

	FPropertyModel PropertyModel(DisplayName, Description);
	PropertyModel.RawDescription = Description;
	PropertyModel.RawShortDescription = Description;
	PropertyModel.Tooltip = Description;
	PropertyModel.CPPType = CPPType;
	PropertyModel.CPPTypeForwardDeclaration = CPPForwardDeclare;
	PropertyModel.Type = DisplayType;
	PropertyModel.DisplayType = DisplayType;

	return PropertyModel;
}

static Kds::DocGen::Models::FFunctionModel CreateFunctionModel(const UFunction* Function)
{
	// Extract basic function information
	const FName FunctionName = FFunctionUtils::GetFunctionName(Function);
	const FString FunctionFullName = FFunctionUtils::GetFunctionFullName(Function);
	const FString FunctionDisplayName = FFunctionUtils::GetFunctionDisplayName(Function);
	const FString FunctionDescription = FFunctionUtils::GetFunctionDescription(Function);
	const FString FunctionRawDescription = FFunctionUtils::GetFunctionRawDescription(Function);
	const FString FunctionRawShortDescription = FFunctionUtils::GetFunctionRawShortDescription(Function);

	// Check blueprint-related properties
	bool bBlueprintCallable = Function->HasAnyFunctionFlags(FUNC_BlueprintCallable);
	bool bBlueprintPure = Function->HasAnyFunctionFlags(FUNC_BlueprintPure);
	bool bBlueprintEvent = Function->HasAnyFunctionFlags(FUNC_BlueprintEvent);

	// Create function model
	Kds::DocGen::Models::FFunctionModel FunctionModel(FunctionName, FunctionDescription);
	FunctionModel.DisplayName = FunctionDisplayName;
	FunctionModel.RawDescription = FunctionRawDescription;
	FunctionModel.RawShortDescription = FunctionRawShortDescription;

	FunctionModel.bBlueprintCallable = bBlueprintCallable;
	FunctionModel.bBlueprintPure = bBlueprintPure;
	FunctionModel.bBlueprintEvent = bBlueprintEvent;

	// Process function parameters
	for (TFieldIterator<FProperty> ParamIt(Function); ParamIt; ++ParamIt)
	{
		const FProperty* Param = *ParamIt;
		const FPropertyModel ParamModel = CreatePropertyModel(Param);
		FunctionModel.AddParameter(ParamModel);
	}

	return FunctionModel;
}


TSharedPtr<Models::FClassModel> FNodeDocsGenerator::InitClassDocXml(const UClass* Class, const bool bExcludeSuper)
{
	UE_LOG(LogKantanDocGen, Log, TEXT("Initializing class doc xml for %s..."), *Class->GetName());

	const FName ClassName = FClassUtils::GetClassName(Class);
	const FString ClassDescription = FClassUtils::GetClassDescription(Class);
	const auto ClassModel = MakeShared<FClassModel>(ClassName, ClassDescription);

	ClassModel->DisplayName = FClassUtils::GetClassDisplayName(Class);
	ClassModel->IncludePath = FClassUtils::GetClassIncludePath(Class);
	ClassModel->ClassTree = FClassUtils::GetClassTree(Class);

	const EFieldIterationFlags FieldIterationFlags =
		bExcludeSuper ? EFieldIteratorFlags::ExcludeSuper : EFieldIterationFlags::IncludeSuper;

	// Process Properties
	for (TFieldIterator<FProperty> It(Class, FieldIterationFlags); It; ++It)
	{
		const FProperty* Prop = *It;
		const FPropertyModel PropertyModel = CreatePropertyModel(Prop);
		ClassModel->AddProperty(PropertyModel);
	}

	// Process Functions
	for (TFieldIterator<UFunction> It(Class, FieldIterationFlags); It; ++It)
	{
		if (const UFunction* Function = *It; !Function->HasAnyFunctionFlags(FUNC_Event))
		{
			const FFunctionModel FunctionModel = CreateFunctionModel(Function);
			ClassModel->AddFunction(FunctionModel);
		}
	}

	// Process Events
	for (TFieldIterator<UFunction> It(Class, FieldIterationFlags); It; ++It)
	{
		if (const UFunction* Function = *It; Function->HasAnyFunctionFlags(FUNC_Event))
		{
			const FFunctionModel EventModel = CreateFunctionModel(Function);
			ClassModel->AddEvent(EventModel);
		}
	}

	return ClassModel;
}

bool FNodeDocsGenerator::UpdateIndexDocWithClass(FWriter* DocFile, const UClass* Class, UObject* SourceObject)
{
	/*check(DocFile);
	check(Class);
	check(SourceObject);

	UE_LOG(LogKantanDocGen, Log, TEXT("Updating index doc with class %s..."), *Class->GetName());

	const auto ClassId = GetClassDocId(Class);
	const auto Classes = DocFile->GetRootNode()->FindChildNode(TEXT("classes"));
	const auto ClassElem = AppendChild(Classes, TEXT("class"));

	AppendChildCDATA(ClassElem, TEXT("id"), ClassId);
	AppendChildCDATA(ClassElem, TEXT("display_name"), FClassUtils::GetClassDocName(Class));
	const bool bIsNative = Class->HasAnyClassFlags(CLASS_Native);

	if (bIsNative)
	{
		TArray<FString> Groups;
		Class->GetClassGroupNames(Groups);
		if (Groups.Num() > 0)
		{
			AppendChildCDATA(ClassElem, TEXT("group"), Groups[0]);
		}
	}
	else
	{
		UBlueprint* BP = Cast<UBlueprint>(SourceObject);
		if (nullptr != BP)
		{
			if (!BP->BlueprintCategory.IsEmpty())
			{
				AppendChildCDATA(ClassElem, TEXT("group"), BP->BlueprintCategory);
			}
			else if (!BP->BlueprintNamespace.IsEmpty())
			{
				AppendChildCDATA(ClassElem, TEXT("group"), BP->BlueprintNamespace);
			}
		}
	}
	AppendChildCDATA(ClassElem, TEXT("type"), bIsNative ? "C++" : "Blueprint");
	AppendChildCDATA(ClassElem, TEXT("description"), FClassUtils::GetClassDescription(Class));*/
	return true;
}

bool FNodeDocsGenerator::UpdateIndexDocWithEnum(FWriter* DocFile, const UEnum* Enum)
{
	/*check(Enum);
	UE_LOG(LogKantanDocGen, Log, TEXT("Updating index doc with enum %s..."), *Enum->GetName());
	auto Classes = DocFile->GetRootNode()->FindChildNode(TEXT("enums"));
	auto ClassElem = AppendChild(Classes, TEXT("enum"));
	AppendChildCDATA(ClassElem, TEXT("display_name"), Enum->CppType);
	AppendChildCDATA(ClassElem, TEXT("description"), Enum->UField::GetToolTipText(false).ToString());

	FString ValuesStr;
	for (int32 Idx = 0; Idx < Enum->NumEnums(); ++Idx)
	{
		if (Idx != 0)
		{
			ValuesStr += "\n";
		}
		FString Name = Enum->GetNameStringByIndex(Idx);
		ValuesStr += Name;
		FString DisplayName = Enum->GetDisplayNameTextByIndex(Idx).ToString();
		if (Name != DisplayName.Replace(TEXT(" "), TEXT("")))
		{
			ValuesStr += " (" + DisplayName + ")";
		}
	}
	AppendChildCDATA(ClassElem, TEXT("values"), ValuesStr);*/
	return true;
}

bool FNodeDocsGenerator::UpdateIndexDocWithStruct(FWriter* DocFile, const UScriptStruct* Struct)
{
	/*check(Struct);
	auto Classes = DocFile->GetRootNode()->FindChildNode(TEXT("structs"));
	auto ClassElem = AppendChild(Classes, TEXT("struct"));

	AppendChildCDATA(ClassElem, TEXT("display_name"), Struct->GetDisplayNameText().ToString());
	AppendChildCDATA(ClassElem, TEXT("description"), Struct->GetToolTipText(false).ToString());

	FXmlNode* Props = AppendChild(ClassElem, TEXT("properties"));
	for (TFieldIterator<FProperty> It(Struct, EFieldIteratorFlags::ExcludeSuper); It; ++It)
	{
		if ((It->PropertyFlags & (CPF_BlueprintVisible | CPF_Edit)) != 0)
		{
			FXmlNode* Prop = AppendChild(Props, TEXT("property"));
			AppendChildCDATA(Prop, TEXT("type"), It->GetCPPType());
			FString DisplayName = It->GetDisplayNameText().ToString();
			AppendChildCDATA(Prop, TEXT("display_name"), DisplayName.Replace(TEXT(" "), TEXT("")));
			FString ToolTip = It->GetToolTipText().ToString();
			if (ToolTip != DisplayName)
			{
				AppendChildCDATA(Prop, TEXT("description"), ToolTip);
			}
		}
	}*/
	return true;
}

bool FNodeDocsGenerator::UpdateClassDocWithNode(FWriter* DocFile, UEdGraphNode* Node)
{
	/*auto NodeId = GetNodeDocId(Node);
	auto Nodes = DocFile->GetRootNode()->FindChildNode(TEXT("nodes"));
	auto NodeElem = AppendChild(Nodes, TEXT("node"));
	AppendChildCDATA(NodeElem, TEXT("id"), NodeId);
	AppendChildCDATA(NodeElem, TEXT("shorttitle"), Node->GetNodeTitle(ENodeTitleType::ListView).ToString());
	AppendChildCDATA(NodeElem, TEXT("description"), FClassUtils::GetNodeDescription(Node));*/
	return true;
}

inline bool ShouldDocumentPin(const UEdGraphPin* Pin)
{
	return !Pin->bHidden;
}

bool FNodeDocsGenerator::GenerateNodeDocs(UK2Node* Node, FNodeProcessingState& State)
{
	SCOPE_SECONDS_COUNTER(GenerateNodeDocsTime);

	/*
	auto NodeDocsPath = State.ClassDocsPath / TEXT("nodes");
	FString DocFilePath = NodeDocsPath / (GetNodeDocId(Node) + TEXT(".xml"));

	const FString FileTemplate = R"xxx(<?xml version="1.0" encoding="UTF-8"?>
<root></root>)xxx";

	FXmlFile File(FileTemplate, EConstructMethod::ConstructFromBuffer);
	auto Root = File.GetRootNode();

	AppendChildCDATA(Root, TEXT("docs_name"), DocsTitle);
	// Since we pull these from the class xml file, the entries are already CDATA wrapped
	AppendChildRaw(Root, TEXT("class_id"), State.ClassDocXml->GetRootNode()->FindChildNode(TEXT("id"))->GetContent());
	AppendChildRaw(Root, TEXT("class_name"),
				   State.ClassDocXml->GetRootNode()->FindChildNode(TEXT("display_name"))->GetContent());

	FString NodeShortTitle = Node->GetNodeTitle(ENodeTitleType::ListView).ToString();
	AppendChildCDATA(Root, TEXT("shorttitle"), NodeShortTitle.TrimEnd());

	FString NodeFullTitle = Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString();
	auto TargetIdx = NodeFullTitle.Find(TEXT("Target is "), ESearchCase::CaseSensitive);
	if (TargetIdx != INDEX_NONE)
	{
		NodeFullTitle = NodeFullTitle.Left(TargetIdx).TrimEnd();
	}

	AppendChildCDATA(Root, TEXT("fulltitle"), NodeFullTitle);
	AppendChildCDATA(Root, TEXT("description"), FClassUtils::GetNodeDescription(Node));
	AppendChildCDATA(Root, TEXT("imgpath"), State.RelImageBasePath / State.ImageFilename);
	AppendChildCDATA(Root, TEXT("category"), Node->GetMenuCategory().ToString());

	auto Inputs = AppendChild(Root, TEXT("inputs"));
	for (auto Pin : Node->Pins)
	{
		if (Pin->Direction == EEdGraphPinDirection::EGPD_Input)
		{
			if (ShouldDocumentPin(Pin))
			{
				auto Input = AppendChild(Inputs, TEXT("param"));

				FString PinName, PinType, PinDesc;
				ExtractPinInformation(Pin, PinName, PinType, PinDesc);

				AppendChildCDATA(Input, TEXT("name"), PinName);
				AppendChildCDATA(Input, TEXT("type"), PinType);
				AppendChildCDATA(Input, TEXT("description"), PinDesc);
			}
		}
	}

	auto Outputs = AppendChild(Root, TEXT("outputs"));
	for (auto Pin : Node->Pins)
	{
		if (Pin->Direction == EGPD_Output)
		{
			if (ShouldDocumentPin(Pin))
			{
				auto Output = AppendChild(Outputs, TEXT("param"));

				FString PinName, PinType, PinDesc;
				ExtractPinInformation(Pin, PinName, PinType, PinDesc);
				AppendChildCDATA(Output, TEXT("name"), PinName);
				AppendChildCDATA(Output, TEXT("type"), PinType);
				AppendChildCDATA(Output, TEXT("description"), PinDesc);
			}
		}
	}

	if (!File.Save(DocFilePath))
	{
		return false;
	}

	if (!UpdateClassDocWithNode(State.ClassDocXml.Get(), Node))
	{
		return false;
	}
	*/

	return true;
}

bool FNodeDocsGenerator::SaveIndexXml(const FString& OutDir) const
{
	auto Path = OutDir / TEXT("index.xml");
	return true;
}

bool FNodeDocsGenerator::SaveClassDocXml(const FString& OutDir) const
{
	UE_LOG(LogKantanDocGen, Log, TEXT("Saving class doc xml..."));
	Writer->Save();
	return true;
}

void FNodeDocsGenerator::AdjustNodeForSnapshot(const UEdGraphNode* Node)
{
	// Hide default value box containing 'self' for Target pin
	if (const auto K2_Schema = Cast<UEdGraphSchema_K2>(Node->GetSchema()))
	{
		if (const auto TargetPin = Node->FindPin(K2_Schema->PN_Self))
		{
			TargetPin->bDefaultValueIsIgnored = true;
		}
	}
}

FString FNodeDocsGenerator::GetNodeDocId(const UEdGraphNode* Node)
{
	// @TODO: Not sure this is right thing to use
	return Node->GetDocumentationExcerptName();
}

#include "BlueprintVariableNodeSpawner.h"
#include "BlueprintDelegateNodeSpawner.h"
#include "K2Node_CallFunction.h"

/*
This takes a graph node object and attempts to map it to the class which the node conceptually belong to.
If there is no special mapping for the node, the function determines the class from the source object.
*/
UClass* FNodeDocsGenerator::GetAssociatedClass(UK2Node* NodeInst, UObject* Source)
{
	// For nodes derived from UK2Node_CallFunction, associate with the class owning the called function.
	if (const auto FuncNode = Cast<UK2Node_CallFunction>(NodeInst))
	{
		if (const auto Func = FuncNode->GetTargetFunction())
		{
			UClass* OwnerClass = Func->GetOwnerClass();
			if (OwnerClass->ClassGeneratedBy)
			{
				if (UBlueprint* ClassBP = Cast<UBlueprint>(OwnerClass->ClassGeneratedBy))
				{
					if (ClassBP && ClassBP->GeneratedClass)
					{
						return ClassBP->GeneratedClass;
					}
				}
			}
			return OwnerClass;
		}
	}

	// Default fallback
	if (const auto SourceClass = Cast<UClass>(Source))
	{
		return SourceClass;
	}
	if (const auto SourceBP = Cast<UBlueprint>(Source))
	{
		return SourceBP->GeneratedClass;
	}

	return nullptr;
}

bool FNodeDocsGenerator::IsSpawnerDocumentable(UBlueprintNodeSpawner* Spawner, const bool bIsBlueprint)
{
	if (nullptr == Spawner)
	{
		return false;
	}
	// Spawners of or deriving from the following classes will be excluded
	static const TSubclassOf<UBlueprintNodeSpawner> ExcludedSpawnerClasses[] = {
		UBlueprintVariableNodeSpawner::StaticClass(), UBlueprintDelegateNodeSpawner::StaticClass(),
		UBlueprintBoundNodeSpawner::StaticClass(), UBlueprintComponentNodeSpawner::StaticClass(),
		UBlueprintBoundEventNodeSpawner::StaticClass()
	};

	// Spawners of or deriving from the following classes will be excluded in a blueprint context
	static const TSubclassOf<UBlueprintNodeSpawner> BlueprintOnlyExcludedSpawnerClasses[] = {
		UBlueprintEventNodeSpawner::StaticClass(),
	};

	// Spawners for nodes of these types (or their subclasses) will be excluded
	static const TSubclassOf<UK2Node> ExcludedNodeClasses[] = {
		UK2Node_DynamicCast::StaticClass(),
		UK2Node_Message::StaticClass(),
	};

	// Function spawners for functions with any of the following metadata tags will also be excluded
	static const FName ExcludedFunctionMeta[] = { TEXT("BlueprintAutocast") };

	static constexpr uint32 PermittedAccessSpecifiers = (FUNC_Public | FUNC_Protected);

	for (const auto ExclSpawnerClass : ExcludedSpawnerClasses)
	{
		if (Spawner->IsA(ExclSpawnerClass))
		{
			return false;
		}
	}

	if (bIsBlueprint)
	{
		for (const auto ExclSpawnerClass : BlueprintOnlyExcludedSpawnerClasses)
		{
			if (Spawner->IsA(ExclSpawnerClass))
			{
				return false;
			}
		}
	}

	for (auto ExclNodeClass : ExcludedNodeClasses)
	{
		if (Spawner->NodeClass->IsChildOf(ExclNodeClass))
		{
			return false;
		}
	}

	if (auto FuncSpawner = Cast<UBlueprintFunctionNodeSpawner>(Spawner))
	{
		auto Func = FuncSpawner->GetFunction();

		// @NOTE: We exclude based on access level, but only if this is not a spawner for a blueprint event
		// (custom events do not have any access specifiers)
		if ((Func->FunctionFlags & FUNC_BlueprintEvent) == 0 && (Func->FunctionFlags & PermittedAccessSpecifiers) == 0)
		{
			return false;
		}

		for (const auto& Meta : ExcludedFunctionMeta)
		{
			if (Func->HasMetaData(Meta))
			{
				return false;
			}
		}
	}

	return true;
}

bool FNodeDocsGenerator::CanBeDocumented(UBlueprintNodeSpawner* Spawner, UObject* SourceObject)
{
	if (!IsSpawnerDocumentable(Spawner, SourceObject->IsA<UBlueprint>()))
	{
		return false;
	}

	const UClass* ObjectAsClass = Cast<UClass>(SourceObject);
	bool bIsDocumentable = true;

	bIsDocumentable &= ObjectAsClass == nullptr || !ObjectAsClass->GetDefaultObject()->IsA<UAnimGraphNode_Base>();

	return bIsDocumentable;
}
