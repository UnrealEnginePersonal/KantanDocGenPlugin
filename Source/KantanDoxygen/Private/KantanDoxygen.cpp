/**
 * @Author: Kasper de Bruin bruinkasper@gmail.com
 * @Date: 2024-11-18 09:46:52
 * @LastEditors: Kasper de Bruin bruinkasper@gmail.com
 * @LastEditTime: 2024-11-18 10:31:16
 * @FilePath: Plugins/developer/docs/KantanDocGenPlugin/Source/KantanDoxygen/Private/KantanDoxygen.cpp
 * @Description: Implementation of the KantanDoxygen module.
 */

#include "KantanDoxygen.h"

#define LOCTEXT_NAMESPACE "KantanDoxygen"

IMPLEMENT_MODULE(FKantanDoxygenModule, KantanDoxygen)

void FKantanDoxygenModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FKantanDoxygenModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}

#undef LOCTEXT_NAMESPACE