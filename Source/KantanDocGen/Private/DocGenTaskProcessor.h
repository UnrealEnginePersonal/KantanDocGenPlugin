// /***********************************************************************************
// *  File:             DocGenTaskProcessor.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "CoreMinimal.h"

#include "DocGenSettings.h"

#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Containers/Queue.h"
#include "CoreMinimal.h"

class ISourceObjectEnumerator;
class FNodeDocsGenerator;

class UBlueprintNodeSpawner;

class FDocGenTaskProcessor final : public FRunnable
{
public:
	FDocGenTaskProcessor();
    ~FDocGenTaskProcessor();
public:
	void QueueTask(const FKantanDocGenSettings& Settings);
	bool IsRunning() const;

public:
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;

protected:
	void QueueTaskInternal(const FKantanDocGenSettings& Settings, const FNotificationInfo& TaskInfo);

	struct FDocGenTask
	{
		FKantanDocGenSettings Settings;
		TSharedPtr<SNotificationItem> Notification;
	};

	struct FDocGenCurrentTask
	{
		TSharedPtr<FDocGenTask> Task;

		TQueue<TSharedPtr<ISourceObjectEnumerator>> Enumerators;
		TSet<FName> Excluded;
		TSet<TWeakObjectPtr<>> Processed;

		TSharedPtr<ISourceObjectEnumerator> CurrentEnumerator;
		TWeakObjectPtr<> SourceObject;
		TQueue<TWeakObjectPtr<UBlueprintNodeSpawner>> CurrentSpawners;

		TUniquePtr<FNodeDocsGenerator> DocGen;
	};

	struct FDocGenOutputTask
	{
		TSharedPtr<FDocGenTask> Task;
	};

protected:
	void ProcessTask(TSharedPtr<FDocGenTask> InTask);

	enum EIntermediateProcessingResult : uint8
	{
		Success,
		SuccessWithErrors,
		UnknownError,
		DiskWriteFailure,
	};

	EIntermediateProcessingResult ProcessIntermediateDocs(const FString& IntermediateDir, const FString& OutputDir,
	                                                      const FString& DocTitle, bool bCleanOutput);

protected:
	TQueue<TSharedPtr<FDocGenTask>> Waiting;
	TUniquePtr<FDocGenCurrentTask> Current;
	TQueue<TSharedPtr<FDocGenOutputTask>> Converting;

	// @NOTE: Using this to sync with module calls from game thread is not 100% okay (we're not atomically testing), but whatever.
	FThreadSafeBool bRunning;

	FThreadSafeBool bTerminationRequest;
};
