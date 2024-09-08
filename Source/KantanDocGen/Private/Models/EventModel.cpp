#include "EventModel.h"

namespace Kds::DocGen::Models
{
	FEventModel::FEventModel(const FName InName, const FString& InFullName, const FString& InDescription,
							 const FString& InImage, const bool bInBlueprintCallable, const bool bInBlueprintEvent) :
		FBaseModel(InName, InDescription), FullName(InFullName), Image(InImage),
		bBlueprintCallable(bInBlueprintCallable), bBlueprintEvent(bInBlueprintEvent)
	{
	}

	void FEventModel::AddParameter(const FPropertyModel&& Parameter)
	{
		Parameters.Add(Parameter);
	}
} // namespace Kds::DocGen::Models
