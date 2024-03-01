#include "StdInc.h"
#include "windows.h"
#include "Debug.h"
#include "Renderer.hpp"

#define ALLOC_STATS_MODULE_NAME "GUI"


DWORD WINAPI MainThread(LPVOID lpReserved)
{
    auto renderer = Renderer::GetSingletonPtr();
    renderer->initialize();

    return 1;
}



BOOL APIENTRY DllMain(HMODULE h_module, uintptr_t dw_reason_for_call, LPVOID lp_reserved)
{
    if (dw_reason_for_call == DLL_PROCESS_ATTACH)
    {
        CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), h_module, NULL, NULL);
        return 1;
    }

    return 0;
}

TRILOGY_API int InitImGuiInterface()
{
    TSA_LOG_DEBUG("Module 'gui' has been initialized");

    return 1;
}
