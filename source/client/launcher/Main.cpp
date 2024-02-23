#include "StdInc.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    SetCurrentProcessExplicitAppUserModelID(L"TrilogyMP");

    #ifdef TRILOGY_DEBUG
        SString loaderDllFilename = "loader_d.dll";
    #else
        SString loaderDllFilename = "loader.dll";
    #endif

    SString launcherPath = PathJoin(GetLaunchPath(), "binaries");
    SString loaderDllPath = PathJoin(launcherPath, loaderDllFilename);

    DWORD dwPrevMode = SetErrorMode(SEM_FAILCRITICALERRORS);
    HMODULE hModule = LoadLibraryW(FromUTF8(loaderDllPath));
    DWORD dwLoadLibraryError = GetLastError();

    if (!hModule)
    {
        SetCurrentDirectoryW(FromUTF8(launcherPath));
        hModule = LoadLibraryW(FromUTF8(loaderDllPath));
        dwLoadLibraryError = GetLastError();

        if (hModule)
        {
            OutputDebugString(SString("Load Library '%s' succeeded on change to directory '%s'", *loaderDllPath, *launcherPath));
        }
    }

    int iReturnCode = 0;

    if (hModule)
    {
        // Find and call DoWinMain
        typedef int (*PFNDOWINMAIN)(HINSTANCE, HINSTANCE, LPSTR, int);
        PFNDOWINMAIN pfnDoWinMain = static_cast<PFNDOWINMAIN>(static_cast<PVOID>(GetProcAddress(hModule, "Run")));
        if (pfnDoWinMain)
            iReturnCode = pfnDoWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

        FreeLibrary(hModule);
    }

    return iReturnCode;
}
