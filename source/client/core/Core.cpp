#include "StdInc.h"
#include <iostream>

void AttachConsoleToApp() {
    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}

BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {

        AttachConsoleToApp();


        std::cout << "Core Injected" << std::endl;
    } else {
        FreeConsole();
    }


    return TRUE;
}
