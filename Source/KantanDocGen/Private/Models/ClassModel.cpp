#include "ClassModel.h"
#include "PropertyModel.h"
#include "EventModel.h"
#include "FunctionModel.h"


namespace Kds::DocGen::Models
{
	FClassModel::FClassModel(const FString& InDocsName, const FString& InId, const FName& InDisplayName,
							 const FString& InDescription) :

		FBaseModel(InDisplayName, InDescription), DocsName(InDocsName), IncludePath(),  Properties(),Id(InId), Events(),
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
	FJsonObject FClassModel::ToJson() const
	{
		FJsonObject OutObject;

		OutObject.SetStringField("Name", this->Name.ToString());
		OutObject.SetStringField("IncludePath", this->IncludePath);
		OutObject.SetStringField("Description", this->Description);

		
		// Properties
		TArray<TSharedPtr<FJsonValue>> JsonPropertiesArray;
		for (const TSharedPtr<FPropertyModel>& Property : this->Properties)
		{
			const TSharedPtr<FJsonObject> PropertyJson = MakeShared<FJsonObject>(Property->ToJson());
			JsonPropertiesArray.Add(MakeShared<FJsonValueObject>(PropertyJson));
		}
		
		OutObject.SetArrayField("Properties", JsonPropertiesArray);

		// Events
		TArray<TSharedPtr<FJsonValue>> JsonEventsArray;
		for (const TSharedPtr<FEventModel>& Event : this->Events)
		{
			const TSharedPtr<FJsonObject> EventJson = MakeShared<FJsonObject>(Event->ToJson());
			JsonEventsArray.Add(MakeShared<FJsonValueObject>(EventJson));
		}

		OutObject.SetArrayField("Events", JsonEventsArray);

		// Functions
		TArray<TSharedPtr<FJsonValue>> JsonFunctionsArray;
		for (const TSharedPtr<FFunctionModel>& Function : this->Functions)
		{
			const TSharedPtr<FJsonObject> FunctionJson = MakeShared<FJsonObject>(Function->ToJson());
			JsonFunctionsArray.Add(MakeShared<FJsonValueObject>(FunctionJson));
		}

		OutObject.SetArrayField("Functions", JsonFunctionsArray);


		return OutObject;		
	}

} // namespace Kds::DocGen::Models
