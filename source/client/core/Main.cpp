#include "StdInc.h"
#include <filesystem>
#include "app/Debug.h"
#include "renderer/Renderer.hpp"

void DisplayConsole()
{
    if (AllocConsole())
    {
        HWND console_hwnd = GetConsoleWindow();
        RECT console_bound = { 900, 420 };
        RECT window_rect;

        SetConsoleTitleA("TRILOGY:MP | Debug");

        GetWindowRect(console_hwnd, &window_rect);
        MoveWindow(console_hwnd, window_rect.left, window_rect.top, console_bound.left, console_bound.top, true);

        SetWindowLong(console_hwnd, GWL_STYLE, GetWindowLong(console_hwnd, GWL_STYLE) | WS_BORDER);
        SetWindowLong(console_hwnd, GWL_EXSTYLE, GetWindowLong(console_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        SetLayeredWindowAttributes(console_hwnd, 0, 230, 2);

        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    }
}

void WaitForDebugger() {
    while (!::IsDebuggerPresent()) {
        printf("Debugger not present\n");
        ::Sleep(100);
    }
}


DWORD WINAPI MainThread(LPVOID lpReserved)
{
    auto renderer = Renderer::GetSingletonPtr();
    renderer->initialize();

    TSA_LOG_DEBUG("Core has been injected to main process.");

    return 1;
}

Core* gs_Core = NULL;
HINSTANCE gs_Module = NULL;

BOOL APIENTRY DllMain(HMODULE h_module, uintptr_t dw_reason_for_call, LPVOID lp_reserved) {


    if (dw_reason_for_call == DLL_PROCESS_ATTACH)
    {
        if (AllocConsole())
        {
            HWND console_hwnd = GetConsoleWindow();
            RECT console_bound = { 900, 420 };
            RECT window_rect;

            SetConsoleTitleA("TRILOGY:MP | Debug");

            GetWindowRect(console_hwnd, &window_rect);
            MoveWindow(console_hwnd, window_rect.left, window_rect.top, console_bound.left, console_bound.top, true);

            SetWindowLong(console_hwnd, GWL_STYLE, GetWindowLong(console_hwnd, GWL_STYLE) | WS_BORDER);
            SetWindowLong(console_hwnd, GWL_EXSTYLE, GetWindowLong(console_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

            SetLayeredWindowAttributes(console_hwnd, 0, 230, 2);

            freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        }

        DisableThreadLibraryCalls(h_module);
        CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), h_module, NULL, NULL);

        return true;
    }

    return false;
}
