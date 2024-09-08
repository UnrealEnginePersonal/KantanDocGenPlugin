#include "FunctionModel.h"


namespace Kds::DocGen::Models
{
	FFunctionModel::FFunctionModel(const FName& InName, const FString& InFullName, const FString& InDescription,
								   const FString& InImage, const bool bInBlueprintCallable,
								   const bool bInBlueprintEvent) :
		FBaseModel(InName, InDescription), FullName(InFullName), Image(InImage),
		bBlueprintCallable(bInBlueprintCallable), bBlueprintPure(false), bBlueprintEvent(bInBlueprintEvent)
	{
	}

	FJsonObject FFunctionModel::ToJson() const
	{
		FJsonObject OutObject;

		OutObject.SetStringField("Name", this->Name.ToString());
		OutObject.SetStringField("FullName", this->FullName);
		OutObject.SetStringField("DisplayName", this->DisplayName);
		OutObject.SetStringField("RawDescription", this->RawDescription);
		OutObject.SetStringField("RawShortDescription", this->RawShortDescription);
		OutObject.SetStringField("Description", this->Description);
		OutObject.SetStringField("Image", this->Image);

		// Flags
		const TSharedPtr<FJsonObject> FunctionFlagsJson = MakeShared<FJsonObject>();
		FunctionFlagsJson->SetBoolField("BlueprintCallable", this->bBlueprintCallable);
		FunctionFlagsJson->SetBoolField("BlueprintPure", this->bBlueprintPure);
		FunctionFlagsJson->SetBoolField("BlueprintEvent", this->bBlueprintEvent);
		OutObject.SetObjectField("Flags", FunctionFlagsJson);

		// Function Parameters
		TArray<TSharedPtr<FJsonValue>> JsonParametersArray;
		for (const TSharedPtr<FPropertyModel>& Parameter : this->Parameters)
		{
			const TSharedPtr<FJsonObject> ParameterJson = MakeShared<FJsonObject>(Parameter->ToJson());
			JsonParametersArray.Add(MakeShared<FJsonValueObject>(ParameterJson));
		}

		OutObject.SetArrayField("Parameters", JsonParametersArray);

		return OutObject;
	}
	void FFunctionModel::AddParameter(const FPropertyModel& Parameter)
	{
		Parameters.Add(MakeShared<FPropertyModel>(Parameter));
	}


} // namespace Kds::DocGen::Models
