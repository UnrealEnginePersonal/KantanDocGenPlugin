#include "ClassModel.h"

#include "PropertyModel.h"
#include "EventModel.h"
#include "FunctionModel.h"

namespace Kds::DocGen::Models
{
	FClassModel::FClassModel(const FString& InDocsName, const FString& InId, const FName& InDisplayName,
							 const FString& InDescription) :

		FBaseModel(InDisplayName, InDescription), DocsName(InDocsName), IncludePath(), Id(InId), Properties(), Events(),
		Functions()
	{
	}
	FClassModel::~FClassModel()
	{
		UE_LOG(LogTemp, Warning, TEXT("FClassModel::~FClassModel()"));
	}

	void FClassModel::AddProperty(const FPropertyModel& Property)
	{
		Properties.Add(MakeUnique<FPropertyModel>(Property));
	}

	void FClassModel::AddEvent(const FEventModel& Event)
	{
		Events.Add(MakeUnique<FEventModel>(Event));
	}

	void FClassModel::AddFunction(const FFunctionModel& Function)
	{
		Functions.Add(MakeUnique<FFunctionModel>(Function));
	}

} // namespace Kds::DocGen::Models
