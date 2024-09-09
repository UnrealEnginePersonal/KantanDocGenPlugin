// /***********************************************************************************
// *  File:             FunctionUtils.h
// *  Project:          Kds_CharacterModule
// *  Author(s):        Kasper de Bruin
// *  Created:          09-09-2024
// *
// *  Copyright (c) 2024  Nightmare Fuel Games
// *  All rights reserved.
// **/


#pragma once

class FFunctionUtils
{
public:
	static FName GetFunctionName(const UFunction* Function);
	
	static FString GetFunctionFullName(const UFunction* Function);
	static FString GetFunctionNameString(const UFunction* Function);
	static FString GetFunctionDisplayName(const UFunction* Function);
	static FString GetFunctionDescription(const UFunction* Function);	
	static FString GetFunctionRawDescription(const UFunction* Function);
	static FString GetFunctionRawShortDescription(const UFunction* Function);
};
