#pragma once

#include "SString.h"
#include "WString.h"
#include "Path.h"

namespace SharedUtil {
    bool IsGTAProcess();
    SString CalculePath(const SString& strPath);
    SString GetParentProcessPathFilename(int pid);
};

using namespace SharedUtil;
