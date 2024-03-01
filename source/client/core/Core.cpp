#include "StdInc.h"
#include "Core.h"
#include <app/Debug.h>
#include "Singleton.h"
#include <Windows.h>
#include <delayimp.h>

#pragma comment(lib, "delayimp")


void LoadModuleInMemory(ModuleLoader& m_Loader, const SString& strName, const SString& strModuleName);

extern Core* gs_Core;

Core::Core()
{
    gs_Core = this;
    m_pModulesLoaded = false;

    LoadModuleInMemory(m_pGUIModule, "GUI", "gui");

    TSA_LOG_DEBUG("Core has been initialized");
};


Core::~Core()
{
    delete gs_Core;

    TSA_LOG_DEBUG("~Core");
}


void Core::CreateGUI()
{
}

void Core::DestroyGUI()
{
    TSA_LOG_DEBUG("Destroying GUI");

    m_pGUIModule.UnloadModule();
}


void Core::SetModulesLoaded(bool bLoaded)
{
    m_pModulesLoaded = bLoaded;
}


void ApplyHooks()
{
}


void LoadModuleInMemory(ModuleLoader& m_Loader, const SString& strName, const SString& strModuleName)
{
    TSA_LOG_DEBUG(SString("Loading module: %s", strName.c_str()));

    SString dllDirectory = GetSystemDllDirectory();

    if (CalculePath("binaries").CompareI(dllDirectory) == false) {
    
        TSA_LOG_CRIT(SString("DllDirectory wrong:  DllDirectory:'%s'  Path:'%s'", *dllDirectory, *CalculePath("binaries")));
        SetDllDirectory(CalculePath("binaries"));
    }

    SString savedCwd = GetSystemCurrentDirectory();

    #ifdef TRILOGY_DEBUG
        SString strModuleFileName = strModuleName + "_d.dll";
    #else
        String strModuleFileName = strModuleName + ".dll";
    #endif



    m_Loader.LoadModule(CalculePath(PathJoin("binaries", strModuleFileName)));

    if (!m_Loader.IsOk())
    {
        SString strMessage("Error loading '%s' module!\n%s", *strName, *m_Loader.GetLastErrorMessage());
        SString strType = "module-not-loadable&name=" + strModuleName;
    
        TSA_LOG_CRIT(strMessage, strType);
    }
    else {
     TSA_LOG_DEBUG(SString("Loaded module: %s", strName.c_str()));
    }
}
