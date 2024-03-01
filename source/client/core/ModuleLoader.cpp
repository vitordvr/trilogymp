#include "StdInc.h"

using std::string;

ModuleLoader::ModuleLoader(const string& ModuleName) : m_bStatus(false)
{
    m_hLoadedModule = 0;
    LoadModule(ModuleName);
}

ModuleLoader::ModuleLoader() : m_bStatus(false)
{
    m_hLoadedModule = 0;
}

ModuleLoader::~ModuleLoader()
{
    UnloadModule();
}

bool ModuleLoader::LoadModule(const string& ModuleName)
{
    m_hLoadedModule = LoadLibrary(ModuleName.c_str());

    if (m_hLoadedModule != NULL)
    {
        m_bStatus = true;
        m_strLastError = "";
    }
    else
    {
        m_bStatus = false;
        DWORD dwError = GetLastError();
        char  szError[2048] = { 0 };
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwError, LANG_NEUTRAL, szError, sizeof(szError), NULL);
        m_strLastError = szError;
    }

    return m_bStatus;
}

void ModuleLoader::UnloadModule()
{
    FreeLibrary(m_hLoadedModule);
    m_hLoadedModule = 0;
    m_bStatus = false;
    m_strLastError = "";
}

PVOID ModuleLoader::GetFunctionPointer(const string& FunctionName)
{
    if (m_bStatus)
    {
        FARPROC fpProcAddr;

        fpProcAddr = GetProcAddress(m_hLoadedModule, FunctionName.c_str());

        return static_cast<PVOID>(fpProcAddr);
    }
    else
        return NULL;
}

const SString& ModuleLoader::GetLastErrorMessage() const
{
    return m_strLastError;
}
