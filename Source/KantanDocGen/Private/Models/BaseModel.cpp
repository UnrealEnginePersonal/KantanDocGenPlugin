#include "BaseModel.h"

namespace Kds::DocGen::Models
{
	FBaseModel::FBaseModel(const FName& InName, const FString& InDescription) : Name(InName), Description(InDescription)
	{
	}
	
	FBaseModel::~FBaseModel()
	{
		// Empty
	}
} // namespace Kds::DocGen::Models
