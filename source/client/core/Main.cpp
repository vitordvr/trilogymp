#include "StdInc.h"

Core* gs_Core = NULL;
HINSTANCE gs_Module = NULL;

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {

        if (AllocConsole())
        {
            freopen("CONIN$", "r", stdin);
            freopen("CONOUT$", "w", stdout);
            freopen("CONOUT$", "w", stderr);
        }

        gs_Core = new Core();
        gs_Module = dll;
    }
    else if (reason == DLL_PROCESS_DETACH) {

        if (IsGTAProcess())
        {
            FreeConsole();
        
            TerminateProcess(GetCurrentProcess(), 0);
        
            if (gs_Core)
            {
                delete gs_Core;
                gs_Core = NULL;
            }
        
            gs_Module = NULL;
        }
    }


    return TRUE;
}
