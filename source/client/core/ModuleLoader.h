#pragma once

#include <windows.h>
#include <string>

class ModuleLoader
{
public:
    ModuleLoader(const std::string& ModuleName);
    ModuleLoader();
    ~ModuleLoader();

    bool LoadModule(const std::string& ModuleName);
    void UnloadModule();

    bool           IsOk() const { return m_bStatus; }
    const SString& GetLastErrorMessage() const;

    PVOID GetFunctionPointer(const std::string& FunctionName);

private:
    HMODULE m_hLoadedModule;
    bool    m_bStatus;
    SString m_strLastError;
};
