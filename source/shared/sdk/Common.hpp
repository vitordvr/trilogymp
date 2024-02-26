#include "Common.h"


#pragma once



bool SharedUtil::IsGTAProcess()
{
    SString strLaunchPathFilename = GetLaunchPathFilename();
    return strLaunchPathFilename.EndsWithI("SanAndreas.exe");
}
