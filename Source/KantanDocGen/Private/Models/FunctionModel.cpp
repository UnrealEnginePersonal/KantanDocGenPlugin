#include "FunctionModel.h"


namespace Kds::DocGen::Models
{
	FFunctionModel::FFunctionModel(const FName& InName, const FString& InFullName, const FString& InDescription,
								   const FString& InImage, const bool bInBlueprintCallable,
								   const bool bInBlueprintEvent) :
		FBaseModel(InName, InDescription), FullName(InFullName), Image(InImage),
		bBlueprintCallable(bInBlueprintCallable), bBlueprintEvent(bInBlueprintEvent)
	{
	}
	
	void FFunctionModel::AddParameter(const FPropertyModel& Parameter)
	{
		Parameters.Add(Parameter);
	}


} // namespace Kds::DocGen::Models
