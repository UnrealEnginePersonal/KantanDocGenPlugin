#include "ClassModel.h"
#include "PropertyModel.h"
#include "FunctionModel.h"


namespace Kds::DocGen::Models
{
	FClassModel::FClassModel(const FName& InName, const FString& InDescription) :
		FBaseModel(InName, InDescription)
	{
	}

	void FClassModel::AddProperty(const FPropertyModel& Property)
	{
		Properties.Add(MakeShared<FPropertyModel>(Property));
	}

	void FClassModel::AddEvent(const FFunctionModel& Event)
	{
		Events.Add(MakeShared<FFunctionModel>(Event));
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
		for (const TSharedPtr<FFunctionModel>& Event : this->Events)
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
