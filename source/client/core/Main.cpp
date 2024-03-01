#include "StdInc.h"
#include <filesystem>
#include "app/Debug.h"
#include <Windows.h>


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


Core* gs_Core = NULL;
HINSTANCE gs_Module = NULL;



BOOL APIENTRY DllMain(HMODULE h_module, uintptr_t dw_reason_for_call, LPVOID lp_reserved)
{

    DisableThreadLibraryCalls(h_module);

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

        gs_Core = new Core();
        gs_Module = h_module;

        SetDllDirectory(CalculePath("binaries"));

        return true;
    }

    return false;
}
