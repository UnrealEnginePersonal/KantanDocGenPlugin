// /***********************************************************************************
// *  File:             ThreadingHelpers.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          06-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/

#pragma once

#include "CoreMinimal.h"

#include "Async/TaskGraphInterfaces.h"

namespace DocGenThreads
{
	template <typename TLambda>
	inline void RunOnGameThread(TLambda Func)
	{
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
			MoveTemp(Func), TStatId(), nullptr, ENamedThreads::GameThread);
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
	}

	template <typename TLambda, typename... TArgs>
	inline auto RunOnGameThreadRetVal(TLambda Func, TArgs&... Args) -> decltype(Func(Args...))
	{
		using TResult = decltype(Func(Args...));

		TResult Result;
		TFunction<void()> NullaryFunc = [&]
		{
			Result = Func(Args...);
		};

		const FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady(
			NullaryFunc, TStatId(), nullptr, ENamedThreads::GameThread);
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

		return Result;
	}
}