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

#include "GameFramework/Actor.h"

#include "Models/ClassModel.h"

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
		TSharedPtr<Kds::DocGen::Models::FClassModel> ClassModel;
		FString ClassDocsPath;
		FString RelImageBasePath;
		FString ImageFilename;

		FNodeProcessingState();
	};

public:
	FNodeDocsGenerator();

	~FNodeDocsGenerator();

public:
	/** Callable only from game thread */
	bool GT_Init(const FString& InDocsTitle, const FString& InOutputDir,
				 UClass* BlueprintContextClass = AActor::StaticClass());

	bool GT_Finalize(const FString& OutputPath);

	UK2Node* GT_DocumentSimpleObject(UObject* SourceObject, const bool bExcludeSuper, FNodeProcessingState& OutState);

	UK2Node* GT_InitializeForSpawner(UBlueprintNodeSpawner* Spawner, UObject* SourceObject, const bool bExcludeSuper,
									 FNodeProcessingState& OutState);
	/**/
	/** Callable from background thread */
	bool GenerateNodeImage(UEdGraphNode* Node, FNodeProcessingState& State);
	bool GenerateNodeDocs(UK2Node* Node, FNodeProcessingState& State);
	/**/

protected:
	bool SaveIndexXml(const FString& OutDir) const;
	bool SaveClassDocXml(const FString& OutDir) const;

	void CleanUp();

	static bool UpdateIndexDocWithClass(Kds::DocGen::FWriter* Writer, const UClass* Class, UObject* SourceObject);
	static bool UpdateIndexDocWithEnum(Kds::DocGen::FWriter* Writer, const UEnum* Enum);
	static bool UpdateIndexDocWithStruct(Kds::DocGen::FWriter* DocFile, const UScriptStruct* Struct);
	static bool UpdateClassDocWithNode(Kds::DocGen::FWriter* DocFile, UEdGraphNode* Node);

	/*static TSharedPtr<FWriter> InitIndexXml(const FString& IndexTitle);*/
	static TSharedPtr<Kds::DocGen::Models::FClassModel> InitClassDocXml(const UClass* Class,
																		const bool bExcludeSuper);
	
	static void AdjustNodeForSnapshot(const UEdGraphNode* Node);	
	static FString GetNodeDocId(const UEdGraphNode* Node);
	static UClass* GetAssociatedClass(UK2Node* NodeInst, UObject* Source);
	static bool IsSpawnerDocumentable(UBlueprintNodeSpawner* Spawner, const bool bIsBlueprint);
	static bool CanBeDocumented(UBlueprintNodeSpawner* Spawner, UObject* SourceObject);

protected:
	TWeakObjectPtr<UBlueprint> DummyBP;
	TWeakObjectPtr<UEdGraph> Graph;
	TSharedPtr<SGraphPanel> GraphPanel;
	TSharedPtr<Kds::DocGen::FWriter> Writer;

public:
	//
	double GenerateNodeImageTime = 0.0;
	double GenerateNodeDocsTime = 0.0;
	//
};
