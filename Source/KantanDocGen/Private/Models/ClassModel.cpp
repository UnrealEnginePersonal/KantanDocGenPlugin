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

	void FClassModel::AddProperty(const FPropertyModel& Property)
	{
		Properties.Add(MakeShared<FPropertyModel>(Property));
	}

	void FClassModel::AddEvent(const FEventModel& Event)
	{
		Events.Add(MakeShared<FEventModel>(Event));
	}

	void FClassModel::AddFunction(const FFunctionModel& Function)
	{
		Functions.Add(MakeShared<FFunctionModel>(Function));
	}

} // namespace Kds::DocGen::Models
