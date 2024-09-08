// /***********************************************************************************
// *  File:             NodeDocsGenerator.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleManager.h"
#include "CoreMinimal.h"
#include "XmlFile.h"

#include "GameFramework/Actor.h"

#include "Models/JsonModel.h"
#include "Generators/Writer.h"
struct FKantanDocGenSettings;

class UClass;
class UBlueprint;
class UEdGraph;
class UEdGraphNode;
class UK2Node;
class UBlueprintNodeSpawner;

class FNodeDocsGenerator
{
public:
	struct FNodeProcessingState
	{
		TSharedPtr<KantanDocGen::Json::Models::FClassModel> ClassModel;
		FString ClassDocsPath;
		FString RelImageBasePath;
		FString ImageFilename;

		FNodeProcessingState()
			: ClassModel(nullptr)
			  , ClassDocsPath()
			  , RelImageBasePath()
			  , ImageFilename()
		{
		}
	};

public:
	FNodeDocsGenerator();

	~FNodeDocsGenerator();

public:
	/** Callable only from game thread */
	bool GT_Init(const FString& InDocsTitle, const FString& InOutputDir,
	             UClass* BlueprintContextClass = AActor::StaticClass());

	bool GT_Finalize(const FString& OutputPath);

	UK2Node* GT_DocumentSimpleObject(UObject* SourceObject, FNodeProcessingState& OutState);

	UK2Node* GT_InitializeForSpawner(UBlueprintNodeSpawner* Spawner, UObject* SourceObject,
	                                 FNodeProcessingState& OutState);
	/**/
	/** Callable from background thread */
	bool GenerateNodeImage(UEdGraphNode* Node, FNodeProcessingState& State);
	bool GenerateNodeDocs(UK2Node* Node, FNodeProcessingState& State);
	/**/
protected:
	bool SaveIndexXml(const FString& OutDir) const;
	bool SaveClassDocXml(const FString& OutDir);

	void CleanUp();

	static bool UpdateIndexDocWithClass(KantanDocGen::Json::FWriter* Writer, const UClass* Class, UObject* SourceObject);
	static bool UpdateIndexDocWithEnum(KantanDocGen::Json::FWriter* Writer, const UEnum* Enum);
	static bool UpdateIndexDocWithStruct(KantanDocGen::Json::FWriter* DocFile, const UScriptStruct* Struct);
	static bool UpdateClassDocWithNode(KantanDocGen::Json::FWriter* DocFile, UEdGraphNode* Node);

	/*static TSharedPtr<FWriter> InitIndexXml(const FString& IndexTitle);*/
	static TSharedPtr<KantanDocGen::Json::Models::FClassModel> InitClassDocXml(
		const UClass* Class, const FString& InDocsTitle);

	static void AdjustNodeForSnapshot(const UEdGraphNode* Node);
	static FString GetClassDocId(const UClass* Class);
	static FString GetNodeDocId(const UEdGraphNode* Node);
	static UClass* GetAssociatedClass(UK2Node* NodeInst, UObject* Source);
	static bool IsSpawnerDocumentable(UBlueprintNodeSpawner* Spawner, const bool bIsBlueprint);
	static bool CanBeDocumented(UBlueprintNodeSpawner* Spawner, UObject* SourceObject);

protected:
	TWeakObjectPtr<UBlueprint> DummyBP;
	TWeakObjectPtr<UEdGraph> Graph;
	TSharedPtr<SGraphPanel> GraphPanel;
	TSharedPtr<KantanDocGen::Json::FWriter> Writer;

public:
	//
	double GenerateNodeImageTime = 0.0;
	double GenerateNodeDocsTime = 0.0;
	//
};
